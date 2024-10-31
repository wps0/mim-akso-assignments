#ifndef GROUPS_H
#define GROUPS_H

#include <stddef.h>

typedef struct group group_t;

/// @brief Creates a group.
group_t *group_make();

/// @brief Creates n groups.
/// @param n The amount of groups to create.
/// @return A pointer to the array of n groups or NULL, if allocation failed.
///  The pointer returned by the function has to be freed manually.
group_t **group_make_n(size_t n);

void group_free(group_t *group);

/// @brief Deallocates n groups starting from gptr.
/// @param gptr A pointer to the groups array to remove.
/// @param n The amount of groups to remove.
void group_free_n(group_t **gptr, size_t n);

void group_update_member_count(group_t **gptr, int delta);

/// @brief Renames a given group.
/// @param gptr A non-null pointer to the group which will be renamed.
/// @param new_name A non-null pointer to the new name. The name will be copied.
/// @return 1 if a group name was modified. 0 if the names are equal.
///  -1 if an error has occurred.
int group_rename(group_t *gptr, char const *new_name);

/// @return A pointer to the original name of gptr. Do not free it.
char const * group_get_name(group_t *gptr);
/// @return A length of the name, null-terminating char inclusive.
size_t group_get_name_length(group_t *gptr);

/// @brief Merges two groups together.
/// @param g1 A group to be merged. It will represent both g1 and g2.
/// @param g2 A pointer to the group to be merged. The pointer will be
///  invalidated.
/// @return 1 if the groups were merged. 0 if groups are aleady the same.
///  -1 if memory allocation failed.
int group_merge(group_t *g1, group_t *g2);

#endif