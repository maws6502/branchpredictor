#include "predictor.h"

double
static_not_taken(Trace *t)
{
        unsigned long passed = 0, correct = 0;
        while (t = t->next) {
                passed++;
                if (!t->taken) correct++;
        }
        
        return (double) correct / (double) passed;
}