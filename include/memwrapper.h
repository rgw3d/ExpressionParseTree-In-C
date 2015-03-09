#pragma once

#include <stdlib.h>

void *mwrealloc(void *ptr, size_t origSz, size_t sz);
void *mwalloc(size_t sz);
void mwfree(void *ptr, size_t sz);

size_t mwAllocated();
size_t mwTotalAllocated();
size_t mwTotalDeallocated();
size_t mwPeakAllocated();

void memstat();
