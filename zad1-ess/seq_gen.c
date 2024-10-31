#include <string.h>

char *gen_seq(size_t n) {
    char *seq = malloc(n+1);
    for (size_t i = 0; i < n; i++)
    {
        seq[i] = '0' + (rand() % 3);
    }
    seq[n] = '\0';
    return seq;
}