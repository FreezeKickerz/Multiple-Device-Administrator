#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <err.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "net.h"
#include "jbod.h"

/* the client socket descriptor for the connection to the server */
int cli_sd = -1;

/* attempts to read n bytes from fd; returns true on success and false on
 * failure */
static bool nread(int fd, int len, uint8_t *buf) {
  int remainingLen = 0;
  while (remainingLen < len){
    int read_check = read(fd, &buf[remainingLen], len - remainingLen);
    if (read_check <= 0){
      return false;
    }
    else{
      remainingLen += read_check;
    }
  }
  return true;
}

/* attempts to write n bytes to fd; returns true on success and false on
 * failure */
static bool nwrite(int fd, int len, uint8_t *buf) {
  if (buf == NULL) {
  	return false;
  }
  if (len == 0) {
  	return false;
  }
  int remainingLen = 0;
  while(remainingLen < len){
    int write_check = write(fd, &buf[remainingLen], len - remainingLen);
    if (write_check <= 0){
      return false;
    }
    else{
      remainingLen += write_check;
    }
  }
  return true;
}

/* attempts to receive a packet from fd; returns true on success and false on
 * failure */
static bool recv_packet(int fd, uint32_t *op, uint16_t *ret, uint8_t *block) {
  uint8_t packet[HEADER_LEN];
  uint16_t packetLen;

  if (!nread(fd, HEADER_LEN, packet)){
    return false;
  }

  memcpy(&packetLen, packet, 2);
	memcpy(op, &packet[2], 4);
	memcpy(ret, &packet[6], 2);

  packetLen = ntohs(packetLen);
  *op = ntohl(*op);
	*ret = ntohs(*ret);

  if (packetLen == (HEADER_LEN + JBOD_BLOCK_SIZE)){
    if (!nread(fd, 256, block)){
      return false;
    }
  }
  return true;
}

/* attempts to send a packet to sd; returns true on success and false on
 * failure */
static bool send_packet(int sd, uint32_t op, uint8_t *block) {
  uint8_t packet[HEADER_LEN];
  uint16_t packetLen;

  if(block == NULL){
    packetLen = HEADER_LEN;
  }
  else{
    packetLen = HEADER_LEN + JBOD_BLOCK_SIZE;
  }

  packetLen = htons(packetLen);
  op = htonl(op);

  memcpy(&packet, &packetLen, 2);
	memcpy(&packet[2], &op, 4);
	nwrite(sd, HEADER_LEN, packet);

  if (block != NULL) {
  if (!nwrite(sd, HEADER_LEN, packet)){
    return false;
  }
}
return true;
}

/* attempts to connect to server and set the global cli_sd variable to the
 * socket; returns true if successful and false if not. */
bool jbod_connect(const char *ip, uint16_t port) {
  
  struct sockaddr_in connection;
  connection.sin_family = AF_INET;
  connection.sin_port = htons(port);

  if (inet_aton(ip, &connection.sin_addr) == 0){
    return false;
  }
  
  cli_sd = socket(AF_INET, SOCK_STREAM, 0);
  if (cli_sd == -1){
    return false;
  }
  if (connect(cli_sd, (const struct sockaddr*) &connection, sizeof(connection)) == -1){
    return false;
  }
  return true;
}

/* disconnects from the server and resets cli_sd */
void jbod_disconnect(void) {
  close(cli_sd);
  cli_sd = -1;
}

/* sends the JBOD operation to the server and receives and processes the
 * response. */
int jbod_client_operation(uint32_t op, uint8_t *block) {
  uint16_t ret;
  send_packet(cli_sd, op, block);
  recv_packet(cli_sd, &op, &ret, block);
  return ret;  
}
