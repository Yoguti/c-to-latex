#ifndef ARENA_H
#define ARENA_H

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    unsigned char* buffer;
    size_t buffer_len;
    size_t off_set;
} Arena;

void arena_init(Arena *arena, size_t size);
void* arena_realloc(Arena *arena, size_t size);
void arena_free(Arena *arena);

#endif