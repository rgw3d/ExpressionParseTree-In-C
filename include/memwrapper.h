#pragma once

#include <stddef.h>

/**
 * Reallocates the memory at a pointer.
 *
 * @param ptr the pointer to the original block of memory.
 * @param origSz the original size of the allocated memory.
 * @param sz the size to reallocate the block of memory to.
 * @return the pointer to the new block of memory.
 */
void *mwrealloc(void *ptr, size_t origSz, size_t sz);
/**
 * Allocates a block of memory.
 *
 * @param sz the size of the memory to allocate.
 * @return the pointer to the allocated block of memory.
 */
void *mwalloc(size_t sz);
/**
 * Deallocates a block of memory.
 *
 * @param ptr the pointer to the block of memory to deallocate.
 * @param sz the size of the block of memory to deallocate.
 */
void mwfree(void *ptr, size_t sz);

/**
 * @return the size of memory that is currently allocated.
 */
size_t mwAllocated();
/**
 * @return the total size of memory that has been allocated.
 */
size_t mwTotalAllocated();
/**
 * @return the total size of memory that has been deallocated.
 */
size_t mwTotalDeallocated();
/**
 * @return the peak size of memory that has been allocated at some point.
 */
size_t mwPeakAllocated();

/**
 * Displays some heap memory usage statistics.
 */
void memstat();
