#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cache.h"

static cache_entry_t *cache = NULL;
static int cache_size = 0;
static int clock = 0;
static int num_queries = 0;
static int num_hits = 0;

bool cacheEnabled = false;

int cache_create(int num_entries) {
  if(cacheEnabled == true){
    return -1;
  }
  if(num_entries < 2 || num_entries > 4096){
    return -1;
  }
  cache_size = num_entries;
  cache = calloc(cache_size, sizeof(cache_entry_t));
  cacheEnabled = true;
  return 1;
}

int cache_destroy(void) {
  if(cacheEnabled){
    free(cache);
    cache = NULL;
    cache_size = 0;
    clock = 0;
    cacheEnabled = false;
    return 1;
  }
  return -1;
}

int cache_lookup(int disk_num, int block_num, uint8_t *buf) {
  if(cacheEnabled == false || cache == NULL || buf == NULL || disk_num > 15 || disk_num < 0 || block_num > 255 || block_num < 0){    
    return -1;
  }
  num_queries++;  
  for (int j = 0; j < cache_size; j++){
    if (cache[j].valid && cache[j].disk_num == disk_num && cache[j].block_num == block_num){      
      num_hits++;
      clock++;
      cache[j].access_time = clock;
      memcpy(buf, cache[j].block, JBOD_BLOCK_SIZE);
      return 1;
    }
  }
  return -1;
}

void cache_update(int disk_num, int block_num, const uint8_t *buf) {
  for(int j = 0; j < cache_size; j++){
    if(cache[j].valid && cache[j].disk_num == disk_num && cache[j].block_num == block_num){
      memcpy(cache[j].block, buf, JBOD_BLOCK_SIZE);
      clock++;
      cache[j].access_time = clock;
    }
  }
}

int cache_insert(int disk_num, int block_num, const uint8_t *buf) {
  int smallestAccessTime;
  int memLocation = -1;
  if(cacheEnabled == false || cache == NULL || buf == NULL || disk_num > 15 || disk_num < 0 || block_num > 255 || block_num < 0){    
    return -1;
  }
  for(int j = 0; j < cache_size; j++){
    if(cache[j].valid == true && cache[j].disk_num == disk_num && cache[j].block_num == block_num){
      return -1;
    }
    if(cache[j].valid == false){
      memLocation = j;
      break;
    }
  }

  if(memLocation == -1){
    smallestAccessTime = cache[0].access_time;
    memLocation = 0;
    for (int j = 1; j < cache_size; j++){
      if(cache[j].access_time < smallestAccessTime){
        smallestAccessTime = cache[j].access_time;
        memLocation = j;
      }
    }
  }
  memcpy(cache[memLocation].block, buf, JBOD_BLOCK_SIZE);
  cache[memLocation].disk_num = disk_num;
  cache[memLocation].block_num = block_num;
  cache[memLocation].valid = true;
  clock++;
  cache[memLocation].access_time = clock;
  return 1;
}

bool cache_enabled(void) {
  return cacheEnabled;
}

void cache_print_hit_rate(void) {
  fprintf(stderr, "Hit rate: %5.1f%%\n", 100 * (float) num_hits / num_queries);
}