#include <cmath>
#include <cstring>

#include "cache_module.h"

extern CacheModule *head_node;

CacheModule::CacheModule(uint32_t blocksize, uint32_t cache_size, uint32_t assoc) {
  this->blocksize = blocksize;
  this->cache_size = cache_size;
  this->assoc = assoc;
  sets = (cache_size)/(blocksize * assoc);

  blockoffsetbits = log2(blocksize);
  indexbits = log2(sets);
  tagoffset = blockoffsetbits + indexbits;
  tagbits = 32 - tagoffset;

  // Initialize the metadata
  metadata = new MetaData*[sets];
  for(uint32_t i=0; i < sets; ++i) {
    metadata[i] = new MetaData[assoc];
    (void)memset(metadata[i], 0, (assoc) * sizeof(MetaData));
  }

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

uint32_t CacheModule::getTagAddr(uint32_t Addr) {
  return (Addr >> (tagoffset));
}
