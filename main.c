#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>


#define MAX_TRACE_LINE_LEN 40 /* 2lazy2count */


struct trace {
        unsigned long addr;     /* assuming all the trace files are 64 bit addresses at max */
        int taken;              /* memory is cheap; native int performance is forever */
        struct trace *next;     /* linked list, seemed appropriate somehow */
};
typedef struct trace Trace;

double
static_taken_eval(Trace *t)
{
        unsigned long passed = 0, correct = 0;
        while (t = t->next) { /* don't question it, don't -Wall it */
                passed++;
                if (t->taken) correct++;
        }

        return (double) correct / (double) passed;
}

double
static_not_taken_eval(Trace *t)
{
        unsigned long passed = 0, correct = 0;
        while (t = t->next) {
                passed++;
                if (!t->taken) correct++;
        }

        return (double) correct / (double) passed;
}

double
dynamic_bm_eval(Trace *t, int n)
{
        unsigned long passed = 0, correct = 0;
        uint8_t *itab = malloc(1L << n + 1); /* no point saving on more memory, we know the usage anyway */
        /* TODO: When less sleepy, see if I'm allocating double the amount of memory needed */
        long mask = (1L << n + 1) - 1;
        /* we are mapping -1:2 -> SNT:ST */

        while (t = t->next) {
                passed++;
                /* first, predict */
                if (itab[t->addr & mask] > 0 && t->taken || itab[t->addr && mask] <= 0 && !t->taken) /* ew */
                        correct++;
                /* then, update */
                if (t->taken && itab[t->addr & mask] < 2)
                        itab[t->addr & mask]++;
                else if (!t->taken && itab[t->addr & mask] > -1)
                        itab[t->addr & mask]--;

        }
        free(itab);

        return (double) correct / (double) passed;
}

int
main(int argc, char *argv[])
{
        /* (poorly) attempt to stick to c89 */
        FILE *trace_file;
        char trace_line[MAX_TRACE_LINE_LEN];
        Trace *trace_root, *t;       

        if (argc != 2 || access(argv[1], R_OK)) {
                printf("Error: invalid arguments\n"); /* enough said */
                return 1;
        }

        trace_file = fopen(argv[1], "r");
        trace_root = malloc(sizeof(Trace)); /* this one's a throwaway, only for the next */
        t = trace_root;

        while (fgets(trace_line, MAX_TRACE_LINE_LEN, trace_file)) {
                t->next = malloc(sizeof(Trace));
                sscanf(trace_line, "0x%lx %d", &t->next->addr, &t->next->taken);
                t = t->next;
                t->next = NULL; /* you never know */
        }

        double st = static_taken_eval(trace_root);
        printf("Static Taken: %lf\n", st);
        double snt = static_not_taken_eval(trace_root);
        printf("Static Not Taken: %lf\n", snt);
        double dbm = dynamic_bm_eval(trace_root, 2);
        printf("Dynamic BM, n=2: %lf\n", dbm);
        dbm = dynamic_bm_eval(trace_root, 4);
        printf("Dynamic BM, n=4: %lf\n", dbm);
        dbm = dynamic_bm_eval(trace_root, 8);
        printf("Dynamic BM, n=8: %lf\n", dbm);
}
