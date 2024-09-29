#pragma once

extern bool streamBuffer_Search(uint32_t, uint32_t &, uint32_t &);

extern void streamBuffer_Sync(CacheModule*, uint32_t, uint32_t);

extern void streamBuffer_Write(CacheModule *, uint32_t);

extern void printStreamBuffer(void);
