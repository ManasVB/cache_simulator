#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>

#include "cache_module.h"
#include "prefetch.h"

using namespace std;

uint32_t Prefetch_N = 0U, Prefetch_M = 0U;
vector<vector<uint32_t>> streamBuffer;
extern uint32_t total_mem_traffic;

bool streamBuffer_Search(uint32_t blockAddr, uint32_t &rowitr, uint32_t &colitr) {
  
  for(int64_t i = (streamBuffer.size() - 1); i >= 0; --i) {
    for(uint32_t j = 0; j < streamBuffer[i].size(); ++j) {
      if(blockAddr == streamBuffer[i][j]) {
        rowitr = i; colitr = j;
        return true;
      }
    }
  }
  return false;
}

void streamBuffer_Sync(CacheModule *ptr, uint32_t rowitr, uint32_t colitr) {

  uint32_t rowbufferSize = streamBuffer[rowitr].size();
  for(uint32_t k = (streamBuffer[rowitr][rowbufferSize-1]+1); k <= streamBuffer[rowitr][rowbufferSize-1] + (colitr+1); ++k) {
    streamBuffer[rowitr].push_back(k);
    ++ptr->prefetches;
    ++total_mem_traffic;
  }

  streamBuffer[rowitr].erase(streamBuffer[rowitr].begin(), streamBuffer[rowitr].begin() + (colitr + 1));

  std::vector<uint32_t> erasedBuffer = streamBuffer[rowitr];
  streamBuffer.erase(streamBuffer.begin() + rowitr);
  streamBuffer.push_back(erasedBuffer);

  assert(streamBuffer.size() <= Prefetch_N);
}

void streamBuffer_Write(CacheModule *ptr, uint32_t blockAddr) {

  std::vector<uint32_t> rowBuffer;
  for(uint32_t j = blockAddr + 1; j <= blockAddr + Prefetch_M; ++j) {
    rowBuffer.push_back(j);
    ++ptr->prefetches;
    ++total_mem_traffic;
  }

  if(streamBuffer.size() == Prefetch_N) {
    streamBuffer.erase(streamBuffer.begin());
  }

  assert(streamBuffer.size() < Prefetch_N);

  streamBuffer.push_back(rowBuffer);

}

void printStreamBuffer() {
    cout << "===== Stream Buffer(s) contents =====" << endl;
    for(auto i = streamBuffer.rbegin(); i != streamBuffer.rend(); ++i) {
      if(!((*i).empty())) {
        for(auto &j: (*i)) {
          printf("%x\t", j);
        }
        printf("\n");
      }
    }
    printf("\n");
}
