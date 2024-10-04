#pragma once

#include <cstdint>
#include "sim.h"
#include "cache_module.h"

/**
 * LRU State Update Function.
 * This function is called whenever there is a HIT in the cache.
 * It implements LRU replacement policy to determine which cache block HIT,
 * and updates the LRU Counter with that associated block.
 */
extern void LRU_StateUpdate(CacheModule*, MetaData, uint32_t, bool);

/**
 * Implements Write-Back Write-Allocate Policy
 * When there is a cache MISS, the block is allocated according to this policy.
 * First check if the particular cache set is full; if it is full and there exists
 * a dirty LRU block, writeback that block to the next level in hierarchy and then
 * perform eviction.
 */
extern void Write_Policy(CacheModule*, uint32_t);
