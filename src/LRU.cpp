#include "LRU.h"

void LRU_StateUpdate(CacheModule *cache_ptr, uint32_t index, uint32_t itr) {

  for(uint32_t j=0; j < cache_ptr->Associativity(); ++j) {
    if((cache_ptr->metadata[index][j]).LRU_Counter < \
      (cache_ptr->metadata[index][itr].LRU_Counter)) {
      ++((cache_ptr->metadata[index][j]).LRU_Counter);
    }
  }

  (cache_ptr->metadata[index][itr].LRU_Counter) = 0U;
}

void LRU_Policy(CacheModule *cache_ptr, uint32_t index, uint32_t tag) {
  uint32_t assoc = cache_ptr->Associativity();

  for(uint32_t j=0; j < assoc; ++j) {
    if((cache_ptr->metadata[index][j]).LRU_Counter == (assoc - 1)) {
      (cache_ptr->metadata[index][j]).tag = tag;
      (cache_ptr->metadata[index][j]).valid_bit = true;
      LRU_StateUpdate(cache_ptr, index, j);
      break;
    }
  }
}
