#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>

#include "flint/flint.h"
#include "flint/fmpz.h"
#include "flint/ulong_extras.h"

#include "mpoly.h"
#include "mat.h"
#include "gmconnection.h"
#include "deformation.h"

#include "flint/padic_mat.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    _randinit(state);

    {
        const char *str = 
            "4  [3 0 0 0] [0 3 0 0] [0 0 3 0] [0 0 0 3] (2  0 4)[1 1 1 0]";
        const long n = atoi(str) - 1;

        mpoly_t P;
        ctx_t ctxFracQt;
        qadic_ctx_t Qq;
        fmpz_t p  = {5L};
        long d    = 1;
        qadic_t t1;
        prec_t prec, prec_in;

        ctx_init_fmpz_poly_q(ctxFracQt);
        qadic_ctx_init_conway(Qq, p, d, 1, 1, "X", PADIC_SERIES);

        qadic_init2(t1, 1);
        qadic_one(t1);

        mpoly_init(P, n + 1, ctxFracQt);
        mpoly_set_str(P, str, ctxFracQt);

        frob(P, ctxFracQt, t1, Qq, &prec, NULL, 1);

        qadic_clear(t1);
        mpoly_clear(P, ctxFracQt);
        ctx_clear(ctxFracQt);
        qadic_ctx_clear(Qq);
    }

    _randclear(state);
    _fmpz_cleanup();
    return EXIT_SUCCESS;
}

