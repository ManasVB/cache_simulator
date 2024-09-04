#include <cmath>

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

}

uint32_t CacheModule::tagAddr(uint32_t Addr) {
  return (Addr >> (this->tagoffset));
}
