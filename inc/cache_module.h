#pragma once

#include <iostream>
#include <cstdint>
#include <vector>

#include "sim.h"

class CacheModule {

  private:

  uint32_t blocksize;
  uint32_t cache_size;
  uint32_t assoc;
  uint32_t sets;

  uint32_t blockoffsetbits;
  uint32_t indexbits;
  uint32_t tagoffset;
  uint32_t tagbits;

  public:

  std::string Cache_Name;

  std::vector<std::vector<MetaData>> metadata;

  uint32_t Cache_Read_Requests, Cache_Write_Requests;
  uint32_t Cache_Read_Miss, Cache_Write_Miss;
  uint32_t Writeback_Nxt_Lvl;
  uint32_t prefetches = 0;
  
  float Miss_Rate = 0.0000f;

  CacheModule *next_node;  // linked-list impl to link to next level of hierarchy

  CacheModule(uint32_t, uint32_t, uint32_t, std::string);
  ~CacheModule() = default;

  uint32_t parseAddress(uint32_t, uint32_t &);

  uint32_t Associativity(void);

  uint32_t BlockOffset(void);

  uint32_t TagOffset(void);

  void PrintCacheContents(void);

};
