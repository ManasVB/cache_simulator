#include <cstdint>

#include "cache_module.h"
#include "request.h"
#include "LRU.h"

using namespace std;

extern CacheModule *head_node;
extern uint32_t total_mem_traffic;

void requestAddr(CacheModule *ptr, uint32_t addr, bool isWrite) {
  
  if(ptr == nullptr) {
    ++total_mem_traffic;
    // cout << "Mem traffic " << total_mem_traffic << endl;
    return;
  }

  uint32_t index = 0U;
  uint32_t tag = 0U;

  tag = ptr->parseAddress(addr, index);
  // printf("Operation ->%d, Address->%x, tag->%x, Index->%x\n", isWrite, addr,tag,index);

  isWrite ? ++(ptr->Cache_Write_Requests) : ++(ptr->Cache_Read_Requests);
  // std::cout << "Write Requests # " << ptr->Cache_Write_Requests << " Read Requests # " << ptr->Cache_Read_Requests;

  // the variable md stands for metadata
  if(!((ptr->metadata[index]).empty())) {
    for(auto &md : ptr->metadata[index]) {
      if(md.valid_bit == true && md.tag == tag) {
        LRU_StateUpdate(ptr, md, index, isWrite);
        // std::cout << "HIT " << std::endl;
        return;
      }
    }
  }

  isWrite ? ++(ptr->Cache_Write_Miss) : ++(ptr->Cache_Read_Miss);

  // std::cout << " Write Miss # " << ptr->Cache_Write_Miss << " Read Miss # " << ptr->Cache_Read_Miss << std::endl;
  
  // Check blocks in the set and if dirty block, evict it
  Write_Policy(ptr, index);

  // If cache miss occurs, read from the next level in the hierarchy
  requestAddr(ptr->next_node, addr, false);

  MetaData m = {.tag = tag, .valid_bit = true, .dirty_bit = isWrite};
  ptr->metadata[index].push_back(m);

  return;
}
