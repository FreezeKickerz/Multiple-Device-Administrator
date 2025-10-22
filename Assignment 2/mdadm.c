#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "jbod.h"
#include "mdadm.h"

uint32_t encode_op(int cmd, int disk_num, int block_num) {
    uint32_t op = 0;
    op |= (cmd << 26);
    op |= (disk_num << 22);
    op |= (block_num << 0);
    return op;
}

int mounted = 0;

int mdadm_mount(void) {
    if (mounted == 1) {
        return -1;
    } else {
        uint32_t op = encode_op(JBOD_MOUNT, 0, 0);
        int rc = jbod_operation(op, NULL);
        if (rc == 0) {
            mounted = 1;
            return 1;
        }
    }
    return -1;
}

int mdadm_unmount(void) {
    if (mounted == 0) {
        return -1;
    } else {
        uint32_t op = encode_op(JBOD_UNMOUNT, 0, 0);
        int rc = jbod_operation(op, NULL);
        if (rc == 0) {
            mounted = 0;
            return 1;
        }
    }
    return -1;
}

void get_address(uint32_t addr, int *disk_num, int *block_num, int *offset) {
    *disk_num = addr / JBOD_DISK_SIZE;
    *block_num = (addr % JBOD_DISK_SIZE) / JBOD_BLOCK_SIZE;
    *offset = (addr % JBOD_DISK_SIZE) % JBOD_BLOCK_SIZE;
}

int mdadm_read(uint32_t addr, uint32_t len, uint8_t *buf) {
    if (mounted == 0) {
        return -1;
    }
    if (buf == NULL && len != 0) {
        return -1;
    }
    if (len > 1024 || addr < 0 || addr + len > 1048576) {
        return -1;
    }

    int disk_num;
    int block_num;
    int addr_offset;
    uint32_t selected_addr = addr;
    int bytes_read = 0;

    while (selected_addr < (addr + len)) {
        
        get_address(selected_addr, &disk_num, &block_num, &addr_offset);
        uint8_t local_buf[JBOD_BLOCK_SIZE];

        int disk_test = jbod_operation(encode_op(JBOD_SEEK_TO_DISK, disk_num, 0), NULL);
        int block_test = jbod_operation(encode_op(JBOD_SEEK_TO_BLOCK, 0, block_num), NULL);
        if ((disk_test != 0) && (block_test != 0)) {
	        return -1;
        }          
        
        if (jbod_operation(encode_op(JBOD_READ_BLOCK, 0, 0), local_buf) == -1) {
            return -1;
        }
        if (addr_offset == 0) {
            if ((len - bytes_read) < JBOD_BLOCK_SIZE) {
                memcpy(&buf[bytes_read], local_buf, (len - bytes_read));
                selected_addr += (len - bytes_read);
                bytes_read += (len - bytes_read);
            } 
            else {
                memcpy(&buf[bytes_read], local_buf, JBOD_BLOCK_SIZE);
                selected_addr += JBOD_BLOCK_SIZE;
                bytes_read += JBOD_BLOCK_SIZE;
            }
        }
        if (addr_offset != 0) {
            if ((len + addr_offset) < JBOD_BLOCK_SIZE) {
                memcpy(buf, (local_buf + addr_offset), len);
                selected_addr += len;
            } 
            else {
                memcpy(buf, (local_buf + addr_offset),(JBOD_BLOCK_SIZE - addr_offset));
                selected_addr += (JBOD_BLOCK_SIZE - addr_offset);
                bytes_read += (JBOD_BLOCK_SIZE - addr_offset);
            }
        }
    }
    return len;
}