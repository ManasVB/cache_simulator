#include <cstdint>

#include "cache_module.h"
#include "read.h"

extern CacheModule *head_node;

CacheStatus readAddress(uint32_t addr) {
  CacheModule *ptr = head_node;
  uint16_t index = 0;
  uint32_t tag = 0;

  printf("Address Read: %x\t",addr);

  // Traverse thru the memory hierarchy
  while(ptr != nullptr) {

    tag = ptr->parseAddress(addr, index);

    printf("Tag->%x, Index->%x\t", tag, index);
    
    // Check for Cache Hit
    for(uint8_t j=0; j < ptr->setAssoc(); ++j) {
      if((ptr->metadata[index][j].valid_bit == true) \
        && (ptr->tagArray[index][j] == tag)) {
          ++(ptr->Cache_Hits);
          std::cout << "HIT # " << ptr->Cache_Hits << std::endl;
          return HIT;
      }
    }
    
    ++(ptr->Cache_Misses);
    std::cout << "MISS # " << ptr->Cache_Misses << std::endl;

    // If there is an invalid block in the set, bring the requested block here. 
    for(uint8_t j=0; j < ptr->setAssoc(); ++j) {
      if((ptr->metadata[index][j].valid_bit == false)) {
        ptr->tagArray[index][j] = tag;
        ptr->metadata[index][j].valid_bit = true;
        return MISS;
      }
    }

    // Implement LRU policy

    
    
    ptr = ptr->next_node;  
  }

  return MISS;
}
