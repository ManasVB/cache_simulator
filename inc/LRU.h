#pragma once

#include <cstdint>
#include "sim.h"
#include "cache_module.h"

extern void LRU_StateUpdate(CacheModule*, MetaData, uint32_t, bool);

extern void Write_Policy(CacheModule*, uint32_t);
