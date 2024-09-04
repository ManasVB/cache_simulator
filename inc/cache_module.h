#pragma once

#include <iostream>
#include <cstdint>

class CacheModule {

  private:

  uint32_t blocksize;
  uint32_t cache_size;
  uint32_t assoc;
  uint32_t sets;

  uint8_t blockoffsetbits;
  uint8_t indexbits;
  uint8_t tagoffset;
  uint8_t tagbits;

  public:

  CacheModule(uint32_t, uint32_t, uint32_t);
  ~CacheModule();

  uint32_t tagAddr(uint32_t);

};
