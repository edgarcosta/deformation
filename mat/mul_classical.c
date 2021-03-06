#include "mat.h"

void _mat_mul_classical(char **rowsC, 
                              char ** const rowsA, char ** const rowsB, 
                              long ell, long m, long n, const ctx_t ctx)
{
    long i, j, k;
    char *x, *y;

    x = malloc(ctx->size);
    ctx->init(ctx, x);

    for (i = 0; i < ell; i++)
        for (j = 0; j < n; j++)
        {
            y = rowsC[i] + j * ctx->size;
            ctx->zero(ctx, y);
            for (k = 0; k < m; k++)
            {
                ctx->mul(ctx, x, rowsA[i] + k * ctx->size, 
                            rowsB[k] + j * ctx->size);
                ctx->add(ctx, y, y, x);
            }
        }

    ctx->clear(ctx, x);
    free(x);
}

void mat_mul_classical(mat_t C, const mat_t A, 
                             const mat_t B, const ctx_t ctx)
{
    if (!(C->m == A->m && A->n == B->m && B->n == C->n))
    {
        printf("ERROR (mat_mul_classical).\n\n");
        abort();
    }

    _mat_mul_classical(C->rows, A->rows, B->rows, A->m, A->n, B->n, ctx);
}

