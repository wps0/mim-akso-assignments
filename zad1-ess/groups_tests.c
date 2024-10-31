#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>

#include "groups.h"

#define TEST_PASS(f) assert(f == 1)
#define TEST_FAIL(f) assert(f != 1)
#define TEST_COMP(f, s) assert(strcmp((f), (s)) == 0)

void print_group(group_t *g) {
    printf("name: \"%s\" (len: %ld)\n", group_get_name(g), group_get_name_length(g));
}

int main() {
    group_t *g1 = group_make();

    // g1 rename
    TEST_COMP(group_get_name(g1), "");
    TEST_PASS(group_rename(g1, "Ala ma kota"));
    TEST_COMP(group_get_name(g1), "Ala ma kota");

    // add more groups
    group_t **groups = group_make_n(3);
    group_t *g2 = groups[0];
    group_t *g3 = groups[1];
    TEST_PASS(group_rename(g3, "g3"));
    // g2 rename
    TEST_COMP(group_get_name(g2), "");
    TEST_PASS(group_rename(g2, " a kot ma ale"));
    TEST_COMP(group_get_name(g2), " a kot ma ale");

    // merge
    TEST_PASS(group_merge(g1, &groups[0]));
    TEST_COMP(group_get_name(g1), "Ala ma kota a kot ma ale");
    TEST_COMP(group_get_name(g3), "g3");

    group_free(g1);
    group_free_n(groups, 3);
    printf("OK!\n");
}