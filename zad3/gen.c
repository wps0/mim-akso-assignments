#include <stdio.h>
#include <assert.h>
#include <malloc.h>

int main(int argc, char const *argv[]) {
    assert(argc == 2);

    printf("Writing to file %s\n", argv[1]);

    FILE *f = fopen(argv[1], "wb");
    assert(f);

    size_t n = 4;
    char *buf = calloc(n, sizeof(char));
    buf[n-1] = 'S';
    assert(fwrite(buf, sizeof(char), n, f) == n);

    fclose(f);

    printf("OK\n");
    return 0;
}