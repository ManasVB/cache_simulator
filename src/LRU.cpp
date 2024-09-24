#include <cstring>
#include <cassert>

#include "LRU.h"
#include "request.h"

using namespace std;

void LRU_StateUpdate(CacheModule* cache_ptr, MetaData md, uint32_t index, bool isWrite) {

  for(auto itr = (cache_ptr->metadata[index]).begin(); itr != (cache_ptr->metadata[index]).end(); ++itr) {
    if(!(memcmp(&(*itr), &md, sizeof(MetaData)))) {
      (cache_ptr->metadata[index]).erase(itr);
      if(isWrite)
        (md.dirty_bit = true);
      (cache_ptr->metadata[index]).push_back(md);
      break;
    }
  }

  assert((cache_ptr->metadata[index]).size() <= cache_ptr->Associativity());
}

void Write_Policy(CacheModule* cache_ptr, uint32_t index) {

  if(cache_ptr->metadata[index].size() == cache_ptr->Associativity()) {
    if((cache_ptr ->metadata[index].front()).dirty_bit) {
      uint32_t writeback_tag = (cache_ptr->metadata[index].front()).tag;
      uint32_t writeback_block_addr = ((writeback_tag << cache_ptr->TagOffset()) | \
        (index << cache_ptr->BlockOffset()));
      requestAddr(cache_ptr->next_node, writeback_block_addr, true);
      ++(cache_ptr->Writeback_Nxt_Lvl);
    }

    (cache_ptr->metadata[index]).erase(cache_ptr->metadata[index].begin());
  }

  assert((cache_ptr->metadata[index]).size() < cache_ptr->Associativity());  
}
