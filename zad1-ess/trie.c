#include "trie.h"

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#include "utils.h"
#include "groups.h"

/// -------- STRUCTURES

struct node_t
{
    char val;

    group_t *group;
    node_t **children;
};

struct trie_t
{
    node_t *root;

    char *alphabet;
    /// @brief Null-terminating char exclusive.
    size_t alphabet_len;
};

/// -------- DECLARATIONS

static node_t *__make_node(trie_t const *trie, char value);
/// @brief Deallocates memory reserved by the subtree with the given root.
/// @param trie A trie this root belongs to.
/// @param root A node whose subtree to free.
/// @return 1 if the tree was touched, 0 otherwise.
static int __free_subtree(trie_t *trie, node_t **root_ptr);

static int __get_pos_in_alphabet(trie_t const *, char);

inline static node_t **__get_next_node(trie_t *trie, node_t *root, char ch);

static node_t **__get_node_from_seq(trie_t *, char const *, bool);

/// @return The number of nodes that belonged to the old_grp.
static void __replace_gid_dfs(trie_t *trie, node_t *root,
                              group_t *new_grp, group_t *old_grp);

/// -------- INTERNAL FUNCTIONS

node_t *__make_node(trie_t const *trie, char value)
{
    node_t *node = calloc(1, sizeof(node_t));
    group_t *group = group_make();
    node_t **children = calloc(trie->alphabet_len, sizeof(node_t *));
    
    if (node == NULL || group == NULL || children == NULL) {
        // allocation failed
        free(node);
        group_free(group);
        free(children);
        node = NULL;
    }
    else {
        node->children = children;
        node->val = value;
        node->group = group;
        group_update_member_count(&node->group, 1);
    }

    return node;
}

int __free_subtree(trie_t *trie, node_t **root_ptr)
{
    if (root_ptr == NULL || *root_ptr == NULL) {
        return 0;
    }

    node_t *root = *root_ptr;
    for (size_t i = 0; i < trie->alphabet_len; i++) {
        __free_subtree(trie, &root->children[i]);
    }

    group_update_member_count(&root->group, -1);

    free_array((void **)root->children, trie->alphabet_len);
    free(root);
    *root_ptr = NULL;

    return 1;
}

int __get_pos_in_alphabet(trie_t const *trie, char c)
{
    for (size_t i = 0; i < trie->alphabet_len; i++)
        if (trie->alphabet[i] == c)
            return i;
    return -1;
}

node_t **__get_next_node(trie_t *trie, node_t *root, char ch)
{
    int ch_pos = __get_pos_in_alphabet(trie, ch);
    return &(root->children[ch_pos]);
}

node_t **__get_node_from_seq(trie_t *trie, char const *seq,
                             bool create_parents)
{
    size_t seq_len = strlen(seq);
    node_t **prefix_node = &trie->root;
    node_t **first_null = NULL;
    size_t i = 0;

    // go down the tree until the nodes representing the sequence exist
    while (i < seq_len) {
        prefix_node = __get_next_node(trie, *prefix_node, seq[i]);

        if (*prefix_node == NULL) {
            first_null = prefix_node;
            break;
        }
        i++;
    }

    if (!create_parents && first_null != NULL) {
        return NULL;
    }

    // create the nonexistent nodes
    while (i < seq_len) {
        if (*prefix_node != NULL)
            prefix_node = __get_next_node(trie, *prefix_node, seq[i]);
        *prefix_node = __make_node(trie, seq[i]);

        if (*prefix_node == NULL) {
            prefix_node = NULL;
            break;
        }

        i++;
    }

    // in case of a memory allocation error, roll back the changes
    if (i < seq_len) {
        __free_subtree(trie, first_null);
    }

    return prefix_node;
}

void __replace_gid_dfs(trie_t *trie, node_t *root,
                       group_t *new_grp, group_t *old_grp)
{
    if (root != NULL) {
        for (size_t i = 0; i < trie->alphabet_len; i++)
            __replace_gid_dfs(trie, root->children[i], new_grp, old_grp);

        if (root->group == old_grp)
            root->group = new_grp;
    }
}

/// -------- FUNCTIONS

trie_t *trie_make(char const *alphabet)
{
    size_t alphabet_len = strlen(alphabet);
    trie_t *trie = calloc(1, sizeof(trie_t));

    if (trie == NULL) {
        // allocation failed
        return NULL;
    }

    trie->alphabet = strdup_safe(alphabet, alphabet_len + 1);
    trie->alphabet_len = alphabet_len;
    // has to be last; only trie->alphabet_len and trie->group_set
    //  fields are used
    trie->root = __make_node(trie, '\0');

    if (trie->alphabet == NULL || trie->root == NULL) {
        trie_free(trie);
        trie = NULL;
    }

    return trie;
}

void trie_free(trie_t *trie)
{
    if (trie != NULL) {
        __free_subtree(trie, &trie->root);

        free(trie->alphabet);
        free(trie);
    }
}

char const *trie_get_alphabet_ptr(trie_t const *trie)
{
    return trie->alphabet;
}

int trie_add_string(trie_t *trie, char const *s)
{
    if (trie_contains_string(trie, s))
        return 0;

    node_t **seq_node = __get_node_from_seq(trie, s, true);
    return seq_node == NULL ? -1 : 1;
}

int trie_contains_string(trie_t *trie, char const *s)
{
    node_t **s_node = __get_node_from_seq(trie, s, false);
    return s_node != NULL;
}

int trie_remove_string(trie_t *trie, char const *s)
{
    if (!trie_contains_string(trie, s))
        return 0;

    node_t **seq_node = __get_node_from_seq(trie, s, false);
    return __free_subtree(trie, seq_node);
}

group_t *trie_get_group(trie_t *trie, char const *s)
{
    node_t **seq_node = __get_node_from_seq(trie, s, false);
    if (seq_node != NULL) {
        return (*seq_node)->group;
    }
    return NULL;
}

int trie_merge_groups(trie_t *trie, group_t *new_grp, group_t *old_grp)
{
    int merge_status = group_merge(new_grp, old_grp);
    if (merge_status == 1) {
        __replace_gid_dfs(trie, trie->root, new_grp, old_grp);
    }
    return merge_status;
}
