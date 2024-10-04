#include <cstdint>

#include "cache_module.h"
#include "request.h"
#include "LRU.h"
#include "prefetch.h"

using namespace std;

extern CacheModule *head_node;
extern uint32_t total_mem_traffic;
extern uint32_t Prefetch_N, Prefetch_M;

void requestAddr(CacheModule *ptr, uint32_t addr, bool isWrite) {
  
  // If reached memory, increment total_mem_traffic
  if(ptr == nullptr) {
    ++total_mem_traffic;
    return;
  }

  uint32_t index = 0U;
  uint32_t tag = 0U;

  // Parse the incoming address based on the cache configuration
  tag = ptr->parseAddress(addr, index);

  // Determine if the incoming request is r/w
  isWrite ? ++(ptr->Cache_Write_Requests) : ++(ptr->Cache_Read_Requests);

  // the variable md stands for metadata
  if(!((ptr->metadata[index]).empty())) {
    for(auto &md : ptr->metadata[index]) {
      if(md.valid_bit == true && md.tag == tag) {

        // This is a Cache HIT, update the LRU counter of the cache blocks
        LRU_StateUpdate(ptr, md, index, isWrite);
        
        if(ptr->next_node == nullptr && Prefetch_N != 0) {
          // This is Cache HIT and Prefetch HIT: Sync the prefetch buffer
          uint32_t rowitr = 0, colitr = 0;        
          if(streamBuffer_Search(addr >> (ptr->BlockOffset()), rowitr, colitr)) {
            streamBuffer_Sync(ptr, rowitr, colitr);
          }
        }
        return;
      }
    }
  }
  
  // Perform write policy to determine if evictions are needed and how to perform them
  Write_Policy(ptr, index);

  if(ptr->next_node == nullptr && Prefetch_N != 0) {
    uint32_t rowitr = 0, colitr = 0;
    if(streamBuffer_Search(addr >> (ptr->BlockOffset()), rowitr, colitr)) {
      // This is Cache MISS and Prefetch HIT: Sync the prefetch buffer
      // Dont read from next memory level, read from prefetch
      streamBuffer_Sync(ptr, rowitr, colitr);
    } else {
      isWrite ? ++(ptr->Cache_Write_Miss) : ++(ptr->Cache_Read_Miss);
      // This is Cache MISS and Prefetch MISS
      // Read from the next memory level, and update prefetch buffer blocks
      streamBuffer_Write(ptr, addr >> (ptr->BlockOffset()));
      requestAddr(ptr->next_node, addr, false);
    }
  } else {
    isWrite ? ++(ptr->Cache_Write_Miss) : ++(ptr->Cache_Read_Miss);
    // If cache miss occurs and prefetch is not enabled for the current cache level,
    // read from the next level in the hierarchy
    requestAddr(ptr->next_node, addr, false);
  }

  // Add the incoming new block to the cache
  MetaData m = {.tag = tag, .valid_bit = true, .dirty_bit = isWrite};
  ptr->metadata[index].push_back(m);

  return;
}
