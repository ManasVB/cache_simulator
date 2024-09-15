#pragma once

#include <cstdint>
#include "cache_module.h"

extern void LRU_StateUpdate(CacheModule*, uint32_t, uint32_t);

extern void LRU_Policy(CacheModule*, uint32_t, uint32_t);
