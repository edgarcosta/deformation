/*
    Copyright (C) 2016, Edgar Costa

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>

#include "flint.h"
#include "fmpz.h"
#include "ulong_extras.h"

#include "mpoly.h"
#include "mat.h"
#include "gmconnection.h"
#include "deformation.h"

#include "padic_mat.h"

int
main(int argc, char * argv[])
{
    FILE * fp;
    int i, result;
    flint_rand_t state;
    if( argc != 2)
    {
        printf("We need one argument, the input file name.\n");
        return EXIT_FAILURE;
    }

    fp = fopen(argv[1], "r");

    
    _randinit(state);
    if( fp != NULL )
    {
        char * str = NULL;
        size_t len = 0;
        ssize_t readQ;
        char * ptr;
        long n, d;
        fmpz_t e, p;
        mpoly_t P;
        ctx_t ctxFracQt;
        qadic_ctx_t Qq;
        qadic_t t1;
        fmpz_poly_t cp;
        prec_t prec, prec_in;
        unsigned long read[3];
        /*
        input_file format:
        p
        d
        e
        polynomial
        */
        for( i = 0; i < 4; i++)
        {
             readQ = getline(&str, &len, fp);
             if( readQ == -1)
                return EXIT_FAILURE;

            if( i < 3)
            {
                read[i]  = strtoul(str, &ptr, 10);
                printf("check %lu\n", read[i]);
            }

        }
        
        

        n = atoi(str) - 1;

        

        fmpz_init(p);
        fmpz_set_ui(p, read[0]);
        d = read[1];

        ctx_init_fmpz_poly_q(ctxFracQt);
        qadic_ctx_init_conway(Qq, p, d, 1, 1, "X", PADIC_SERIES);

        qadic_init2(t1, 1);
        
        fmpz_init(e);
        if(d != 1)
        {
            qadic_gen(t1, Qq);
            fmpz_set_ui(e,read[2]);

            qadic_pow(t1, t1, e, Qq);

        }
        else
        {
            qadic_set_ui(t1, read[2], Qq);
        }
        mpoly_init(P, n + 1, ctxFracQt);
        mpoly_set_str(P, str, ctxFracQt);
        
        fmpz_poly_init(cp);
        frob_out(cp, P, ctxFracQt, t1, Qq, &prec, NULL, 1, 0);
        
        fmpz_clear(p);
        fmpz_clear(e);
        fmpz_poly_clear(cp);
        qadic_clear(t1);
        mpoly_clear(P, ctxFracQt);
        ctx_clear(ctxFracQt);
        qadic_ctx_clear(Qq);
    }
    else
        return EXIT_FAILURE;

    _randclear(state);
    _fmpz_cleanup();
    return EXIT_SUCCESS;
}

