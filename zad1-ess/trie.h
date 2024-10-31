#ifndef TRIE_H
#define TRIE_H

#include <stddef.h>

#include "groups.h"

/// @brief A type that represents an uncompressed trie.
struct trie_t;
struct node_t;

typedef struct trie_t trie_t;
typedef struct node_t node_t;


trie_t *trie_make(char const *alphabet);
void trie_print(trie_t const *trie);
void trie_free(trie_t *trie);
char const *trie_get_alphabet_ptr(trie_t const *trie);

/// @brief Adds every preffix of a non-empty string s to the given tree.
/// @param s A non-empty string ending with \0.
/// @return 1 if a prefix was added to the tree, 0 if no prefix was added
///  and -1 if an error occurred.
int trie_add_string(trie_t *trie, char const *s);
/// @brief Checks if a trie contains a null-terminated string.
/// @param s A non-empty null-terminated string.
/// @return 1 if seq is present in the trie, 0 otherwise.
int trie_contains_string(trie_t *trie, char const *s);
/// @brief Removes a non-empty null-terminated string from a trie
/// @param s A non-empty null-terminated string.
/// @return 1 if a sequence was removed from the trie, 0 otherwise.
int trie_remove_string(trie_t *trie, char const *s);

/// @param s A non-empty null-terminated string.
/// @return NULL if the sequence does not exist, a pointer to
///  the group of the node representing the string s otherwise.
group_t *trie_get_group(trie_t *trie, char const *s);
/// @brief Merges the groups in a trie and updates the groups module accordingly.
/// @return 1 if the groups were merged. 0 if groups are the same.
///  -1 if memory allocation failed.
int trie_merge_groups(trie_t *trie, group_t *new_grp, group_t *old_grp_ptr);

#endif