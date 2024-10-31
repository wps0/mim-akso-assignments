#include <string.h>
#include <stddef.h>
#include <malloc.h>

#include "utils.h"

char *strdup_safe(char const *s, size_t s_len)
{
    char *s_dup = malloc(s_len * sizeof(char));
    
    if (s_dup != NULL) {
        strcpy(s_dup, s);
    }

    return s_dup;
}

void **calloc_array(size_t len, size_t el_size)
{
    void **arr = calloc(len, sizeof(size_t));

    if (arr != NULL) {
        // allocate len elements
        for (size_t i = 0; i < len; i++) {
            arr[i] = calloc(1, el_size);
            if (arr[i] == NULL) {
                break;
            }
        }

        if (arr[len-1] == NULL) {
            // memory allocation failed
            for (size_t i = 0; i < len; i++) {
                free(arr[i]);
            }
            free(arr);
            arr = NULL;
        }
    }

    return arr;
}

void free_array(void **arr, size_t len) {
    for (size_t i = 0; i < len; i++) {
        free(arr[i]);
    }
    free(arr);
}