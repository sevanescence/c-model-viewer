#ifndef MK_VEC_H
#define MK_VEC_H

/// Dynamic array implementation. Might implement in-place allocator for optimization.

// sample
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define VEC_RESIZE_MULTIPLIER 2
#define INITIAL_CAPACITY 5

#define Vec(TYPE) Vec_##TYPE
#define VEC_INIT {}

// Vector implementation. DOES NOT FREE MANAGED RESOURCES.
// Caveat with macro generic: Cannot use multi-word data types (unsigned int, const char *, etc.). Use typedefs.
#define VEC_DECLARE(TYPE)                                       \
    typedef struct Vec_##TYPE {                                 \
        TYPE *          data;                                   \
        size_t          size;                                   \
        size_t          capacity;                               \
    } Vec_##TYPE;                                               \
                                                                \
    void vec_##TYPE##_resize(Vec_##TYPE *vec);                  \
    inline void vec_##TYPE##_check_for_resize(Vec_##TYPE *vec); \
    void vec_##TYPE##_push_back(Vec_##TYPE *vec, TYPE value);   \
    void vec_##TYPE##_destroy(Vec_##TYPE *const vec);           \
    /* Size must be greater than current array size. */         \
    void vec_##TYPE##_reserve(Vec_##TYPE *vec, size_t size);    \

#define VEC_RESIZE(TYPE, vec_ptr) vec_##TYPE##_resize(vec_ptr)
#define VEC_CHECK_FOR_RESIZE(TYPE, vec_ptr) vec_##TYPE##_check_for_resize(vec_ptr)
#define VEC_PUSH_BACK(TYPE, vec_ptr, value) vec_##TYPE##_push_back(vec_ptr, value)
#define VEC_DESTROY(TYPE, vec_ptr) vec_##TYPE##_destroy(vec_ptr)
#define VEC_RESERVE(TYPE, vec_ptr, size) vec_##TYPE##_reserve(vec_ptr, size)

#define VEC_DEFINE(TYPE)                                                                \
    void vec_##TYPE##_resize(Vec_##TYPE *vec) {                                         \
        size_t new_capacity = vec->capacity * VEC_RESIZE_MULTIPLIER + INITIAL_CAPACITY; \
        TYPE *tmp = malloc(new_capacity * sizeof (TYPE));                               \
        memcpy(tmp, vec->data, vec->size * sizeof (TYPE));                              \
        free(vec->data);                                                                \
        vec->data = tmp;                                                                \
        vec->capacity = new_capacity;                                                   \
    }                                                                                   \
    inline void vec_##TYPE##_check_for_resize(Vec_##TYPE *vec) {                        \
        if (vec->size >= vec->capacity) {                                               \
            VEC_RESIZE(TYPE, vec);                                                      \
        }                                                                               \
    }                                                                                   \
    void vec_##TYPE##_push_back(Vec_##TYPE *vec, TYPE value) {                          \
        VEC_CHECK_FOR_RESIZE(TYPE, vec);                                                \
        vec->data[vec->size] = value;                                                   \
        vec->size += 1;                                                                 \
    }                                                                                   \
    void vec_##TYPE##_destroy(Vec_##TYPE *vec) {                                        \
        free(vec->data);                                                                \
        vec->capacity = 0;                                                              \
        vec->size = 0;                                                                  \
    }                                                                                   \
    void vec_##TYPE##_reserve(Vec_##TYPE *vec, size_t size) {                           \
        TYPE *buf = malloc(size * sizeof (TYPE));                                       \
        memcpy(buf, vec->data, vec->size * sizeof (TYPE));                              \
        free(vec->data);                                                                \
        vec->data = buf;                                                                \
        vec->capacity = size;                                                           \
    }                                                                                   \

#endif