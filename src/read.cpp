#include <cstdint>

#include "cache_module.h"
#include "read.h"
#include "LRU.h"

extern CacheModule *head_node;

void requestAddr(CacheModule *ptr, uint32_t addr) {
  
  if(ptr == nullptr)
    return;

  uint32_t index = 0;
  uint32_t tag = 0;
  uint32_t assoc = ptr->Associativity();

  tag = ptr->parseAddress(addr, index);
  printf("Tag->%x, Index->%x\t", tag, index);

  for(uint32_t j=0; j < assoc; ++j) {
    if((ptr->metadata[index][j].valid_bit == true) \
      && (ptr->metadata[index][j].tag == tag)) {
        ++(ptr->Cache_Hits);
        LRU_StateUpdate(ptr, index, j);
        std::cout << "HIT # " << ptr->Cache_Hits << std::endl;
        return;
    }
  }
  ++(ptr->Cache_Misses);

  // requestAddr(ptr->next_node, addr);

  LRU_Policy(ptr, index, tag);



  std::cout << "MISS # " << ptr->Cache_Misses << std::endl;
  return;

}
