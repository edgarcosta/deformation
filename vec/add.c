#include "vec.h"

void _vec_add(char *res, const char *vec1, const char *vec2, long n, 
              const ctx_t ctx)
{
    long i;

    for (i = 0; i < n; i++)
        ctx->add(ctx, res + i * ctx->size, vec1 + i * ctx->size, vec2 + i * ctx->size);
}

