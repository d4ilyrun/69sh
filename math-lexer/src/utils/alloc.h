#pragma once

#include <stdlib.h>
#include <string.h>

static inline void *xmalloc(size_t size)
{
    void *res = malloc(size);
    if (res == NULL && size != 0)
        abort();
    return res;
}

static inline void *xrealloc(void *ptr, size_t size)
{
    void *res = realloc(ptr, size);
    if (res == NULL && size != 0)
        abort();
    return res;
}

static inline void *zalloc(size_t size)
{
    void *res = xmalloc(size);
    memset(res, 0, size);
    return res;
}
