#include <stdint.h>
#include <stdlib.h>
#include "predictor.h"

double
dynamic_bm(Trace *t, int n)
{
        unsigned long passed = 0, correct = 0;
        /* no point saving on more memory, we know the usage anyway */
        uint8_t *itab = malloc(1L << n + 1); 
        /* TODO: when less sleepy, check if this is 2x the memory we need */

        long mask = (1L << n + 1) - 1;
        /* we are mapping -1:2 -> SNT:ST */

        while (t = t->next) {
                int i = t->addr & mask;
                passed++;
                /* first, predict */
                if (itab[i] > 0 && t->taken || itab[i] <= 0 && !t->taken) /* ew */
                        correct++;
                /* then, update */
                if (t->taken && itab[i] < 2)
                        itab[i]++;
                else if (!t->taken && itab[i] > -1)
                        itab[i]--;

        }

        free(itab);

        return (double) correct / (double) passed;
}