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
  // uint32_t assoc = ptr->Associativity();

  tag = ptr->parseAddress(addr, index);
  printf("Operation ->%d, Address->%x, tag->%x, Index->%x\n", isWrite, addr,tag,index);

  isWrite ? ++(ptr->Cache_Write_Requests) : ++(ptr->Cache_Read_Requests);
  std::cout << "Write Requests # " << ptr->Cache_Write_Requests << " Read Requests # " << ptr->Cache_Read_Requests;

  // the variable md stands for metadata
  if(!((ptr->metadata[index]).empty())) {
    for(auto &md : ptr->metadata[index]) {
      if(md.valid_bit == true && md.tag == tag) {
        LRU_StateUpdate(ptr, md, index, isWrite);
        std::cout << "HIT " << std::endl;
        return;
      }
    }
  }



  // for(uint32_t j=0; j < assoc; ++j) {
  //   if((ptr->metadata[index][j].valid_bit == true) 
  //     && (ptr->metadata[index][j].tag == tag)) {
  //       LRU_StateUpdate(ptr, index, j, isWrite);
  //       std::cout << "HIT " << std::endl;
  //       return;
  //   }
  // }

  isWrite ? ++(ptr->Cache_Write_Miss) : ++(ptr->Cache_Read_Miss);

  std::cout << " Write Miss # " << ptr->Cache_Write_Miss << " Read Miss # " << ptr->Cache_Read_Miss << std::endl;
  
  // If cache miss occurs, read from the next level in the hierarchy
  requestAddr(ptr->next_node, addr, false);

  // Take the address block and place it according to LRU policy
  MetaData m = {.tag = tag, .valid_bit = true};
  LRU_Policy(ptr, m, index, isWrite);

  return;
}
