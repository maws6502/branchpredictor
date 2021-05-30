/* EE318 Assignment 1
 * Aditya Goturu <aditya18203@mechyd.ac.in>
 * 18XJ1A0203
 */
#include "predictor.h"

double
static_taken(Trace *t)
{
        unsigned long passed = 0, correct = 0;
        while (t = t->next) { /* don't question it, don't -Wall it */
                passed++;
                if (t->taken) correct++;
        }

        return (double) correct / (double) passed;
}