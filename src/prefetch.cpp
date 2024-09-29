#include <iostream>
#include <vector>
#include <cstdint>

#include "prefetch.h"

using namespace std;

uint32_t Prefetch_N = 0U, Prefetch_M = 0U;
vector<vector<uint32_t>> streamBuffer;

void StreamBufferSearch(uint32_t BlockAddr) {
  
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
