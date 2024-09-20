#include <cmath>
#include <cstring>
#include <cstdint>
#include <vector>

#include "cache_module.h"

using namespace std;

extern CacheModule *head_node;

void CacheModule::CacheModule_Init(uint32_t blocksize, uint32_t cache_size, uint32_t assoc, std::string name) {

  if(cache_size == 0)
    return;

  // Set cache name
  Cache_Name = name;
  
  // Set the cache parameters
  this->blocksize = blocksize;
  this->cache_size = cache_size;
  this->assoc = assoc;
  sets = (cache_size)/(blocksize * assoc);

  blockoffsetbits = log2(blocksize);
  indexbits = log2(sets);
  tagoffset = blockoffsetbits + indexbits;
  tagbits = 32 - tagoffset;

  // Resize metadata so that no. of rows equal no. of sets
  metadata.resize(sets);

  // Initialize cache read/write and miss count to 0
  Cache_Read_Miss = Cache_Write_Miss = Cache_Read_Requests = Cache_Write_Requests = Writeback_Nxt_Lvl = 0;

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

uint32_t CacheModule::Associativity() {
  return this->assoc;
}

uint32_t CacheModule::BlockOffset() {
  return this->blockoffsetbits;
}

uint32_t CacheModule::TagOffset() {
  return this->tagoffset;
}

void CacheModule::PrintCacheContents() {
  
  cout << "===== " << Cache_Name << " contents " << "===== "<<endl;

  for(uint32_t i = 0; i < sets; ++i) {
    if(!(metadata[i].empty())) {
      printf("set\t%u:\t", i);
      for (auto j = metadata[i].rbegin(); j != metadata[i].rend(); ++j) {
        printf("%x ",(*j).tag);
        (*j).dirty_bit ? printf("D\t") : printf("\t");
      }
      cout << endl;
    }
  }

  cout << endl;
}
