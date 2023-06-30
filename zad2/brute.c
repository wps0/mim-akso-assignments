#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

uint64_t f(uint64_t i, uint64_t n) {
    return 64*i*i/n;
}

void sum(int64_t *x, size_t n) {
    __int128_t buf = 0;
    
    for (int i = 0; i < n; i++) {
        buf += x[i];
        x[i] = 0;

        // wrzuc do x
        for (uint64_t j = f(i, n); j < f(i+1, n); j++) {
            // printf("%s\n", qtoa(buf));
            x[j / 64] |= (buf & 1) * (1ULL << (j % 64));
            buf >>= 1;
        }
    }
}

