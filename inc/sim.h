#pragma once

typedef struct {
   uint32_t BLOCKSIZE;
   uint32_t L1_SIZE;
   uint32_t L1_ASSOC;
   uint32_t L2_SIZE;
   uint32_t L2_ASSOC;
   uint32_t PREF_N;
   uint32_t PREF_M;
} cache_params_t;

typedef struct {
   uint32_t tag;
   bool valid_bit;
   bool dirty_bit;
   uint32_t LRU_Counter;
} MetaData;

typedef enum {
   HIT = true,
   MISS = false
} CacheStatus;
