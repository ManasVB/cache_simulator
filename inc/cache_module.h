#pragma once

#include <iostream>
#include <cstdint>

#include "sim.h"

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

  MetaData **metadata;
  uint32_t **tagArray;

  uint32_t Cache_Hits, Cache_Misses;

  CacheModule *next_node;  // linked-list impl to link to next level of hierarchy

  CacheModule(uint32_t, uint32_t, uint32_t);
  ~CacheModule() = default;

  uint32_t parseAddress(uint32_t, uint16_t &);

  uint32_t setAssoc(void);

};
