#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "memwrapper.h"
#include "switches.h"

static size_t netAllocated = 0;
static size_t totalAllocated = 0;
static size_t totalDeallocated = 0;
static size_t peakAllocated = 0;

void *mwrealloc(void *ptr, size_t origSz, size_t sz) {
	void *newPtr = realloc(ptr, sz);

	// Update the memory counters
	size_t diff = sz - origSz;
	if (diff > 0) {
		totalDeallocated -= diff;
	} else {
		totalAllocated += diff;
	}

	netAllocated += diff;

	if (peakAllocated < netAllocated) {
		peakAllocated = netAllocated;
	}

	if (PRINT_MEM_DEBUG_MSGS) {
		printf("[MWRAP] Reallocated %4zd bytes at %p to %4zd bytes at %p\n",
				origSz, ptr, sz, newPtr);
	}

	return newPtr;
}

void *mwalloc(size_t sz) {
	// Update memory counters
	netAllocated += sz;
	totalAllocated += sz;

	if (peakAllocated < netAllocated) {
		peakAllocated = netAllocated;
	}

	void *ptr = malloc(sz);

	if (PRINT_MEM_DEBUG_MSGS) {
		printf("[MWRAP]   Allocated %4zd bytes at %p\n", sz, ptr);
	}

	return ptr;
}

void mwfree(void *ptr, size_t sz) {
	// Update memory counters
	netAllocated -= sz;
	totalDeallocated += sz;

	free(ptr);

	if (PRINT_MEM_DEBUG_MSGS) {
		printf("[MWRAP] Deallocated %4zd bytes at %p\n", sz, ptr);
	}
}

size_t mwAllocated() {
	return netAllocated;
}
size_t mwTotalAllocated() {
	return totalAllocated;
}
size_t mwTotalDeallocated() {
	return totalDeallocated;
}
size_t mwPeakAllocated() {
	return peakAllocated;
}

void memstat() {
	if (PRINT_MEM_DEBUG_MSGS == 0 && PRINT_MEM_STATS == 0) {
		return;
	}
	printf("[MWRAP] Memory Statistics\n");
	printf("------------------------------------------------\n");
	printf("[MWRAP]         Allocated memory: %8zd bytes\n", mwAllocated());
	printf("[MWRAP]   Total allocated memory: %8zd bytes\n",
			mwTotalAllocated());
	printf("[MWRAP] Total deallocated memory: %8zd bytes\n",
			mwTotalDeallocated());
	printf("[MWRAP]    Peak allocated memory: %8zd bytes\n", mwPeakAllocated());
	printf("------------------------------------------------\n");
}
