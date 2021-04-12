#include <stdint.h>
#include <stdlib.h>
#include "predictor.h"

double
dynamic_tournament(Trace *t, int n)
{
        unsigned long passed = 0, correct = 0, gshare = 0;
        /* if N is too big to fit this, we have other problems */
        int8_t *itab_bm = calloc(1L << n + 1, 1);
        int8_t *itab_gs = calloc(1L << n + 1, 1);
        int8_t *itab_tr = calloc(1L << n + 1, 1);
        /* TODO: when less sleepy, check if this is 2x the memory we need */
        /* we are mapping 0:3 -> SNT:ST */
        /* similarly, 0:3 -> Strongly BM:Strongly BM+Gshare */

        while (t = t->next) {
                int i_bt = t->addr & ((1L << n + 1) - 1); /* same index for the tournament table and the bm table */
                int i_gs = (t->addr ^ gshare) & ((1L << n + 1) - 1);
                int8_t *itab;
                int i, update_correct;
                passed++;
                /* first, predict */
                if (itab_tr[i_bt] >= 2) {
                        itab = itab_gs;
                        i = i_gs;
                        update_correct = 1;
                }
                else {
                        itab = itab_bm;
                        i = i_bt;
                        update_correct = -1;
                }
                if (t->taken ? itab[i] >= 2 : itab[i] <= 1) { /* if our prediction is accurate, we first update the relevant tournament table */
                        correct++;
                        itab_tr[i_bt] += update_correct;
                        if (itab_tr[i_bt] > 3) itab_tr[i_bt] = 3; /* saturate manually */
                        if (itab_tr[i_bt] < 0) itab_tr[i_bt] = 0; /* saturate manually */
                } else {
                        itab_tr[i_bt] -= update_correct;
                        if (itab_tr[i_bt] > 3) itab_tr[i_bt] = 3; /* saturate manually */
                        if (itab_tr[i_bt] < 0) itab_tr[i_bt] = 0; /* saturate manually */
                }
                /* then, update both tables */
                if (t->taken && itab_bm[i_bt] < 3)
                        itab_bm[i_bt]++;
                if (!t->taken && itab_bm[i_bt] > 0)
                        itab_bm[i_bt]--;
                if (t->taken && itab_gs[i_gs] < 3)
                        itab_gs[i_gs]++;
                if (!t->taken && itab_gs[i_gs] > 0)
                        itab_gs[i_gs]--;
                /* finally update gshare */
                gshare <<= 1;
                gshare |= 1L & t->taken; /* little bit of sanity checking, as we have no input validation, so &ding with 1*/
        }

        free(itab_bm);
        free(itab_gs);

        return (double) correct / (double) passed;
}