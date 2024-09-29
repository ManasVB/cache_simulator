#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>

#include "prefetch.h"

using namespace std;

uint32_t Prefetch_N = 0U, Prefetch_M = 0U;
vector<vector<uint32_t>> streamBuffer;

bool streamBuffer_Search(uint32_t blockAddr, uint32_t &rowitr, uint32_t &colitr) {
  
  for(int32_t i = (streamBuffer.size() - 1); i >= 0; --i) {
    for(uint32_t j = 0; j < streamBuffer[i].size(); ++j) {
      if(blockAddr == streamBuffer[i][j]) {
        rowitr = i; colitr = j;
        return true;
      }
    }
  }
  return false;
}

void streamBuffer_Sync(uint32_t rowitr, uint32_t colitr) {

  uint32_t rowbufferSize = streamBuffer[rowitr].size();
  for(uint32_t k = (streamBuffer[rowitr][rowbufferSize-1]+1); k <= streamBuffer[rowitr][rowbufferSize-1] + (colitr+1); ++k) {
    streamBuffer[rowitr].push_back(k);
  }

  streamBuffer[rowitr].erase(streamBuffer[rowitr].begin(), streamBuffer[rowitr].begin() + (colitr + 1));

  std::vector<uint32_t> erasedBuffer = streamBuffer[rowitr];
  streamBuffer.erase(streamBuffer.begin() + rowitr);
  streamBuffer.push_back(erasedBuffer);

  assert(streamBuffer.size() <= Prefetch_N);
}

void streamBuffer_Write(uint32_t blockAddr) {

  std::vector<uint32_t> rowBuffer;
  for(uint32_t j = blockAddr + 1; j <= blockAddr + Prefetch_M; ++j) {
    rowBuffer.push_back(j);
  }

  if(streamBuffer.size() == Prefetch_N) {
    streamBuffer.erase(streamBuffer.begin());
  }

  assert(streamBuffer.size() < Prefetch_N);

  streamBuffer.push_back(rowBuffer);

}

void printStreamBuffer() {

  if(Prefetch_N != 0) {
    cout << "===== Stream Buffer(s) contents =====" << endl;
    for(auto i = streamBuffer.rbegin(); i != streamBuffer.rend(); ++i) {
      if(!((*i).empty())) {
        for(auto &j: (*i)) {
          printf("%x\t", j);
        }
        printf("\n");
      }
    }
  }
}
