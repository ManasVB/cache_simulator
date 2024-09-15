#include "LRU.h"
#include "request.h"

void LRU_StateUpdate(CacheModule *cache_ptr, uint32_t index, uint32_t itr, bool isWrite) {

  for(uint32_t j=0; j < cache_ptr->Associativity(); ++j) {
    if((cache_ptr->metadata[index][j]).LRU_Counter < \
      (cache_ptr->metadata[index][itr].LRU_Counter)) {
      ++((cache_ptr->metadata[index][j]).LRU_Counter);
    }
  }

  (cache_ptr->metadata[index][itr].LRU_Counter) = 0U;

  // If write request, set the dirty bit
  isWrite ? ((cache_ptr->metadata[index][itr].dirty_bit) = true) : \
    ((cache_ptr->metadata[index][itr].dirty_bit) = false);
}

void LRU_Policy(CacheModule *cache_ptr, uint32_t addr, uint32_t index, uint32_t tag, bool isWrite) {
  uint32_t assoc = cache_ptr->Associativity();

  for(uint32_t j=0; j < assoc; ++j) {
    if((cache_ptr->metadata[index][j]).LRU_Counter == (assoc - 1)) {

      // If block if dirty perform writeback to next stage
      if((cache_ptr->metadata[index][j]).dirty_bit) {
        uint32_t writeback_tag = (cache_ptr->metadata[index][j]).tag;
        uint32_t writeback_block_addr = (writeback_tag << cache_ptr->TagOffset()) | (index << cache_ptr->BlockOffset());
        requestAddr(cache_ptr->next_node, writeback_block_addr, true);
        ++(cache_ptr->Writeback_Nxt_Lvl);
      }

      (cache_ptr->metadata[index][j]).tag = tag;
      (cache_ptr->metadata[index][j]).valid_bit = true;
      // (cache_ptr->metadata[index][j]).dirty_bit = false;

      LRU_StateUpdate(cache_ptr, index, j, isWrite);
      break;
    }
  }
}
