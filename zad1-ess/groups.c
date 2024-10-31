#include <stddef.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

#include "groups.h"
#include "utils.h"

/// -------- STRUCTURES

struct group
{
    size_t member_count;

    char *name;
    /// @brief A length of the name, null-terminating char inclusive.
    size_t name_len;
};

/// -------- DECLARATIONS

inline static void __group_construct(group_t *group, char *name,
                                     size_t name_len);

/// -------- INTERNAL FUNCTIONS

inline void __group_construct(group_t *group, char *name, size_t name_len)
{
    group->member_count = 0;
    group->name_len = name_len;
    group->name = name;
}

/// @brief The naming scheme is compliant with the assignment specification.
char *__concatenate_group_names(group_t const *g1, group_t const *g2)
{
    char *new_name = calloc(g1->name_len + g2->name_len - 1, sizeof(char));
    
    if (new_name != NULL) {
        strcpy(new_name, g1->name);
        strcat(new_name, g2->name);
    }

    return new_name;
}

/// -------- FUNCTIONS

group_t *group_make()
{
    group_t *group = calloc(1, sizeof(group_t));
    char *name = calloc(1, sizeof(char));

    if (group == NULL || name == NULL) {
        free(group);
        free(name);
        group = NULL;
    }
    else {
        __group_construct(group, name, 1);
    }

    return group;
}

group_t **group_make_n(size_t n)
{
    group_t **groups = (group_t **) calloc_array(n, sizeof(group_t));
    char **names = (char **) calloc_array(n, sizeof(char));
    
    if (n == 0 || groups == NULL || names == NULL) {
        free(groups);
        free(names);
        return NULL;
    }

    for (size_t i = 0; i < n; i++) {
        __group_construct(groups[i], names[i], 1);
    }

    free(names);
    return groups;
}

void group_free_n(group_t **gptr, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        group_free(gptr[i]);
    }
    
    free(gptr);
}

void group_free(group_t *group)
{
    if (group != NULL) {
        free(group->name);
    }
    free(group);
}

void group_update_member_count(group_t **gptr, int delta)
{
    size_t *member_count = &(*gptr)->member_count;
    *member_count += delta;

    if (*member_count == 0) {
        group_free(*gptr);
        *gptr = NULL;
    }
}

int group_rename(group_t *gptr, char const *new_name)
{
    char *old_name_ptr = gptr->name;
    size_t new_name_len = strlen(new_name)+1;
    
    if (strcmp(old_name_ptr, new_name) == 0) {
        // names are equal
        return 0;
    }

    // names differ
    char *dup = strdup_safe(new_name, new_name_len);
    if (dup == NULL) {
        // memory allocation error
        return -1;
    }
    
    free(old_name_ptr);
    gptr->name = dup;
    gptr->name_len = new_name_len;
    return 1;
}

char const *group_get_name(group_t *gptr)
{
    return gptr->name;
}

size_t group_get_name_length(group_t *gptr)
{
    return gptr->name_len;
}

int group_merge(group_t *g1, group_t *g2)
{
    if (g1 == g2) {
        return 0;
    }
    char *new_name = __concatenate_group_names(g1, g2);
    if (new_name == NULL) {
        return -1;
    }

    free(g1->name);
    g1->name = new_name;
    g1->name_len += g2->name_len - 1;
    g1->member_count += g2->member_count;
    group_free(g2);

    return 1;
}
