#include "seq.h"

#include <malloc.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#include "trie.h"
#include "groups.h"

/// -------- MACROS

#define REQUIRE_ALLOCATION_SUCCESS_OR_CALL(ptr, fun)  \
    if ((ptr) == NULL) {                              \
        fun;                                          \
        errno = ENOMEM;                               \
        return NULL;                                  \
    }                                                 
#define REQUIRE_ALLOCATION_SUCCESS(ptr)                 \
    REQUIRE_ALLOCATION_SUCCESS_OR_CALL(ptr, asm("nop"))

#define VALIDATE_INPUT_ARGS_OR_RETURN(seq, str, ret)  \
    if (!__validate_input_seq_and_str(seq, str)) {    \
        errno = EINVAL;                               \
        return (ret);                                 \
    }                                                 \

#define SET_ERRNO_ON_FAIL_AND_RETURN(status, err)  \
    {                                              \
        int st = (status);                         \
        if (st == -1) {                            \
            errno = (err);                         \
        }                                          \
        return st;                                 \
    }

/// -------- STRUCTURES

struct seq
{
    trie_t *trie;
};

/// -------- DECLARATIONS

inline static bool __validate_input_string(trie_t const *trie, char const *s,
                                           bool skip_char_validation);
inline static bool __validate_input_seq(seq_t const *seq);
inline static bool __validate_input_seq_and_str(seq_t const *seq,
                                                char const *s);

/// -------- INTERNAL FUNCTIONS

inline bool __validate_input_string(trie_t const *trie, char const *s,
                                    bool skip_char_validation)
{
    if (s == NULL || s[0] == '\0') {
        return false;
    }

    bool is_valid = true;
    if (!skip_char_validation) {
        for (size_t i = 0; s[i] != '\0'; i++) {
            if (strrchr(trie_get_alphabet_ptr(trie), s[i]) == NULL) {
                is_valid = false;
                break;
            }
        }
    }

    return is_valid;
}

inline bool __validate_input_seq(seq_t const *seq)
{
    return seq != NULL;
}

inline bool __validate_input_seq_and_str(seq_t const *seq, char const *s)
{
    return __validate_input_seq(seq) &&
           __validate_input_string(seq->trie, s, false);
}

/// -------- FUNCTIONS

seq_t *seq_new(void)
{
    seq_t *seq_set = calloc(1, sizeof(seq_t));
    REQUIRE_ALLOCATION_SUCCESS(seq_set);
    seq_set->trie = trie_make("012");
    REQUIRE_ALLOCATION_SUCCESS_OR_CALL(seq_set->trie, seq_delete(seq_set));

    return seq_set;
}

void seq_delete(seq_t *p)
{
    if (p != NULL) {
        trie_free(p->trie);
        free(p);
    }
}

int seq_add(seq_t *p, char const *s)
{
    VALIDATE_INPUT_ARGS_OR_RETURN(p, s, -1);
    SET_ERRNO_ON_FAIL_AND_RETURN(trie_add_string(p->trie, s), ENOMEM);
}

int seq_remove(seq_t *p, char const *s)
{
    VALIDATE_INPUT_ARGS_OR_RETURN(p, s, -1);
    return trie_remove_string(p->trie, s);
}

int seq_valid(seq_t *p, char const *s)
{
    VALIDATE_INPUT_ARGS_OR_RETURN(p, s, -1);
    return trie_contains_string(p->trie, s);
}

int seq_set_name(seq_t *p, char const *s, char const *n)
{
    VALIDATE_INPUT_ARGS_OR_RETURN(p, s, -1);
    if (!__validate_input_string(p->trie, n, true)) {
        errno = EINVAL;
        return -1;
    }

    group_t *gptr = trie_get_group(p->trie, s);
    if (gptr == NULL) {
        return 0;
    }
    SET_ERRNO_ON_FAIL_AND_RETURN(group_rename(gptr, n), ENOMEM);
}

char const *seq_get_name(seq_t *p, char const *s)
{
    VALIDATE_INPUT_ARGS_OR_RETURN(p, s, NULL);

    group_t *gptr = trie_get_group(p->trie, s);
    if (gptr == NULL) {
        errno = 0;
        return NULL;
    }

    char const *name = group_get_name(gptr);
    if (name[0] == '\0') {
        errno = 0;
        return NULL;
    }
    return name;
}

int seq_equiv(seq_t *p, char const *s1, char const *s2)
{
    VALIDATE_INPUT_ARGS_OR_RETURN(p, s1, -1);
    VALIDATE_INPUT_ARGS_OR_RETURN(p, s2, -1);

    group_t *g1 = trie_get_group(p->trie, s1);
    group_t *g2 = trie_get_group(p->trie, s2);
    if (g1 == NULL || g2 == NULL) {
        // at least one of the strings isn't in the trie
        return 0;
    }

    SET_ERRNO_ON_FAIL_AND_RETURN(trie_merge_groups(p->trie, g1, g2), ENOMEM);
}
