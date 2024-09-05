#include <cmath>
#include <cstring>

#include "cache_module.h"

CacheModule::CacheModule(uint32_t blocksize, uint32_t size, uint32_t assoc) {
  this->blocksize = blocksize;
  this->cache_size = size;
  this->assoc = assoc;
  this->sets = (size)/(blocksize * assoc);

  this->blockoffsetbits = log2(this->blocksize);
  this->indexbits = log2(this->sets);
  this->tagoffset = this->blockoffsetbits + this->indexbits;
  this->tagbits = 32 - this->tagoffset;

  // Initialize the metadata
  this->metadata = new MetaData*[this->sets];
  for(uint32_t i=0; i < this->sets; ++i) {
    this->metadata[i] = new MetaData[this->assoc];
    (void)memset(this->metadata[i], 0, (this->assoc) * sizeof(MetaData));
  }
}

uint32_t CacheModule::getTagAddr(uint32_t Addr) {
  return (Addr >> (this->tagoffset));
}
