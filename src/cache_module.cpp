#include <cmath>
#include <cstring>

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

  // Initialize the metadata and tagArray
  metadata = new MetaData*[sets];
  tagArray = new uint32_t*[sets];
  for(uint32_t i=0; i < sets; ++i) {
    metadata[i] = new MetaData[assoc];
    (void)memset(metadata[i], 0, assoc * sizeof(MetaData));

    tagArray[i] = new uint32_t[assoc];
    (void)memset(tagArray[i], 0, assoc * sizeof(uint32_t));
  }

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

uint32_t CacheModule::parseAddress(uint32_t Addr, uint16_t &index) {
  
  uint32_t mask = (1 << tagoffset) - 1;
  index = (Addr & mask) >> blockoffsetbits;
  return (Addr >> (tagoffset));
}

uint32_t CacheModule::setAssoc(void) {
  return this->assoc;
}
