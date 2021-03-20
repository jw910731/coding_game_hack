#pragma once
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>

bool detour_func(uint64_t src, int src_len, uint64_t des, uint64_t *n_src);