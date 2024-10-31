#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

/// @brief Duplicates the given string.
/// @return A pointer to a copy of the input string or NULL,
///  if an error occurred.
char *strdup_safe(char const *s, size_t s_len);

/// @brief Allocates an array of len elements. Each element
///  is allocated separately.
/// @param len Number of elements.
/// @param el_size Size of each element.
/// @return A pointer to the array or NULL, if an error occurred.
void **calloc_array(size_t len, size_t el_size);

/// @brief Frees an array of length len allocated by calloc_array.
/// @param len Length of array.
void free_array(void **arr, size_t len);

#endif