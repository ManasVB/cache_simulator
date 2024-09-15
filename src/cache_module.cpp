#include <cmath>
#include <cstring>
#include <cstdint>

#include "cache_module.h"

extern CacheModule *head_node;

CacheModule::CacheModule(uint32_t blocksize, uint32_t cache_size, uint32_t assoc) {
  
  // Set the cache parameters
  this->blocksize = blocksize;
  this->cache_size = cache_size;
  this->assoc = assoc;
  sets = (cache_size)/(blocksize * assoc);
  std::cout << "Sets = " << sets << std::endl;

  blockoffsetbits = log2(blocksize);
  indexbits = log2(sets);
  tagoffset = blockoffsetbits + indexbits;
  tagbits = 32 - tagoffset;

  // Initialize the metadata
  metadata = new MetaData*[sets];
  for(uint32_t i=0; i < sets; ++i) {
    metadata[i] = new MetaData[assoc];
  }

  for(uint32_t i=0; i < sets; ++i) {
    for(uint32_t j=0;j < assoc; ++j) {
      metadata[i][j].valid_bit = false;
      metadata[i][j].LRU_Counter = j;
    }
  }

  // for(uint32_t i=0; i < sets; ++i) {
  //   for(uint32_t j=0;j<assoc; ++j) {
  //     std::cout << metadata[i][j].LRU_Counter << " ";
  //   }
  //   std::cout<<std::endl;
  // }

  // Initialize hits and misses to zero
  Cache_Hits = Cache_Misses = 0;

  // Cache linked list implementation
  if(head_node == nullptr) {
    head_node = this;
    this->next_node = nullptr;
  } else {
    CacheModule *ptr = head_node;
    while(ptr->next_node!=nullptr)
      ptr = ptr->next_node;
    ptr->next_node = this;
    this->next_node = nullptr;
  }
}

uint32_t CacheModule::parseAddress(uint32_t Addr, uint32_t &index) {
  
  uint32_t mask = (1 << tagoffset) - 1;
  index = (Addr & mask) >> blockoffsetbits;
  return (Addr >> (tagoffset));
}

uint32_t CacheModule::Associativity(void) {
  return this->assoc;
}
