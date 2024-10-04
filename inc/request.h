#pragma once

#include <cstdint>

/**
 * This is the most important function in the entire program.
 * It first accepts requests from the CPU with 3 arguments:
 * Cache pointer, Address and r/w(0/1).
 * It then parses the incoming address, searches the cache and stream buffer
 * and performs necessary actions for all three combinations:
 * Cache HIT/Stream HIT; Cache MISS/Stream HIT; Cache MISS/Stream MISS
 */
extern void requestAddr(CacheModule*, uint32_t, bool);
