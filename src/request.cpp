#include <cstdint>

#include "cache_module.h"
#include "request.h"
#include "LRU.h"

extern CacheModule *head_node;
extern uint32_t total_mem_traffic;

void requestAddr(CacheModule *ptr, uint32_t addr, bool isWrite) {
  
  if(ptr == nullptr) {
    ++total_mem_traffic;
    return;
  }

  uint32_t index = 0;
  uint32_t tag = 0;
  uint32_t assoc = ptr->Associativity();

  tag = ptr->parseAddress(addr, index);
  printf("Tag->%x, Index->%x\t", tag, index);

  isWrite ? ++(ptr->Cache_Write_Requests) : ++(ptr->Cache_Read_Requests);

  for(uint32_t j=0; j < assoc; ++j) {
    if((ptr->metadata[index][j].valid_bit == true) \
      && (ptr->metadata[index][j].tag == tag)) {
        LRU_StateUpdate(ptr, index, j, isWrite);
        std::cout << "HIT " << std::endl;
        return;
    }
  }

  isWrite ? ++(ptr->Cache_Write_Miss) : ++(ptr->Cache_Read_Miss);
  
  // If cache miss occurs, read from the next level in the hierarchy
  requestAddr(ptr->next_node, addr, false);

  // Take the address block and place it according to LRU policy
  LRU_Policy(ptr, index, tag, isWrite);

  std::cout << "MISS # " << ptr->Cache_Read_Miss << std::endl;
  return;
}
