/* EE318 Assignment 1
 * Aditya Goturu <aditya18203@mechyd.ac.in>
 * 18XJ1A0203
 */
#include <stdint.h>
#include <stdlib.h>
#include "predictor.h"

double
dynamic_bm(Trace *t, int n)
{
        unsigned long passed = 0, correct = 0;
        /* no point saving on more memory, we know the usage anyway */
        int8_t *itab = calloc(1L << n + 1, 1); 
        /* TODO: when less sleepy, check if this is 2x the memory we need */
        /* we are mapping 0:3 -> SNT:ST */

        while (t = t->next) {
                int i = t->addr & ((1L << n + 1) - 1);
                passed++;
                /* first, predict */
                if (t->taken ? itab[i] >= 2 : itab[i] <= 1)
                        correct++;
                /* then, update */
                if (t->taken && itab[i] < 3)
                        itab[i]++;
                if (!t->taken && itab[i] > 0)
                        itab[i]--;
        }

        free(itab);

        return (double) correct / (double) passed;
}