#include "detour.h"
#include <stdio.h>
#include <memory.h>

// src len shound at least for 12 bytes
bool detour_func(uint64_t src, int src_len, uint64_t des, uint64_t *n_src)
{
    int seglen = ( (src_len / 512) + 1 ) * 512;

    uint64_t *code = (uint64_t*)mmap(NULL, seglen, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (code == MAP_FAILED)
    {
        perror("[MAP ERR]");
        return false;
    }

    for (int i = 0 ; i != 4096 ; ++i)
    {
        if (mprotect((void*)(src - i), 4096, PROT_EXEC|PROT_READ|PROT_WRITE) == 0)
        {
            break;
        }
        if (i == 4095)
        {
            perror("[OFFSET]");
            return false;
        }
    }

    uint64_t new_src_st = src + src_len;

    memcpy(code, (void*)src, src_len);
    ((uint8_t*)code+src_len)[0] = 0x48;
    ((uint8_t*)code+src_len)[1] = 0xb8;
    memcpy((uint8_t*)code + src_len + 2, &new_src_st, 8);
    ((uint8_t*)code+src_len)[10] = 0xff;
    ((uint8_t*)code+src_len)[11] = 0xe0;
    
    
    memset((void*)src, 0x90, src_len);
    uint8_t *overwcd = (uint8_t *)src;
    
    overwcd[0] = 0x48;
    overwcd[1] = 0xb8;
    memcpy(overwcd + 2, &des, 8);
    overwcd[10] = 0xff;
    overwcd[11] = 0xe0;
    
    *n_src = (uint64_t)code;
    if (n_src == 0)
    {
        _exit(-1);
    }
    return true;
}