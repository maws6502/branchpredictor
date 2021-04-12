#include <stdint.h>
#include <stdlib.h>
#include "predictor.h"

double
dynamic_bm_gshare(Trace *t, int n)
{
        unsigned long passed = 0, correct = 0, gshare = 0;
        /* if N is too big to fit this, we have other problems */
        int8_t *itab = calloc(1L << n + 1, 1); 
        /* TODO: when less sleepy, check if this is 2x the memory we need */
        /* we are mapping 0:3 -> SNT:ST */

        while (t = t->next) {
                int i = (t->addr ^ gshare) & ((1L << n + 1) - 1);
                passed++;
                /* first, predict */
                if (t->taken ? itab[i] >= 2 : itab[i] <= 1)
                        correct++;
                /* then, update */
                if (t->taken && itab[i] < 3)
                        itab[i]++;
                if (!t->taken && itab[i] > 0)
                        itab[i]--;
                /* finally update gshare */
                gshare <<= 1;
                gshare |= 1L & t->taken; /* little bit of sanity checking, as we have no input validation, so &ding with 1*/
        }

        free(itab);

        return (double) correct / (double) passed;
}