#pragma once

/**
 * This function is used to find a particular block in the prefetch stream.
 * As we are searching for the entire stream buffer and all the buffers,
 * it performs a "linear search" till it finds the desired block inside any of
 * the stream buffers.
 * Return: true: if block is found; else return false
 */
extern bool streamBuffer_Search(uint32_t, uint32_t &, uint32_t &);

/**
 * Whenever there is a HIT in the stream buffer, the stream buffer has to be synced,
 * meaning we need to evict {stream_buffer.begin() to block_location} and then fill
 * the buffer with {block_location+1 to bloc_location+x} where 'x' is the number of evicted
 * blocks.
 */
extern void streamBuffer_Sync(CacheModule*, uint32_t, uint32_t);

/**
 * This function is similar to 'streamBuffer_Sync' except this is only used when there is 
 * a stream buffer MISS. That is all the stream buffers are searched and we get a false output from
 * 'streamBuffer_Search'. At that time, search for the LRU stream buffer, clear it and fill the stream
 * buffer with 'M' blocks ie, (missed_block+1 to missed_block+m)
 */
extern void streamBuffer_Write(CacheModule *, uint32_t);

/**
 * Print the contents of the stream buffer.
 */
extern void printStreamBuffer(void);
