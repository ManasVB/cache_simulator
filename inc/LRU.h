#pragma once

#include <cstdint>
#include "sim.h"
#include "cache_module.h"

extern void LRU_StateUpdate(CacheModule*, MetaData, uint32_t, bool);

extern void LRU_Policy(CacheModule*, MetaData, uint32_t, bool);
