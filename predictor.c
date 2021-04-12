#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "predictor.h"

int
main(int argc, char *argv[])
{
        /* (poorly) attempt to stick to c89. 
         * this attempt would have failed by the around 4 AM  */
        FILE *trace_file;
        Trace *trace_root, *t;
        double res;
        char trace_line[MAX_TRACE_LINE_LEN];

        if (argc != 2 || access(argv[1], R_OK)) {
                printf("Error: invalid arguments\n"); /* enough said */
                return 1;
        }

        trace_file = fopen(argv[1], "r");
        trace_root = malloc(sizeof(Trace)); /* this one's a throwaway, only for the next */
        t = trace_root;

        while (fgets(trace_line, MAX_TRACE_LINE_LEN, trace_file)) { /* never trust fscanf */
                t->next = malloc(sizeof(Trace));
                sscanf(trace_line, "0x%lx %d", &t->next->addr, &t->next->taken);
                t = t->next;
                t->next = NULL; /* you never know */
        }

        res = static_taken(trace_root);
        printf("ST\t\t\t\t\t\t%lf\n", res);
        res = static_not_taken(trace_root);
        printf("SNT\t\t\t\t\t\t%lf\n", res);
        res = dynamic_bm(trace_root, 2);
        printf("Dynamic BM\t\t\tn=2\t\t%lf\n", res);
        res = dynamic_bm(trace_root, 4);
        printf("Dynamic BM\t\t\tn=4\t\t%lf\n", res);
        res = dynamic_bm(trace_root, 8);
        printf("Dynamic BM\t\t\tn=8\t\t%lf\n", res);
        res = dynamic_bm_gshare(trace_root, 2);
        printf("Dynamic BM+GSHARE\t\tn=2\t\t%lf\n", res);
        res = dynamic_bm_gshare(trace_root, 4);
        printf("Dynamic BM+GSHARE\t\tn=4\t\t%lf\n", res);
        res = dynamic_bm_gshare(trace_root, 8);
        printf("Dynamic BM+GSHARE\t\tn=8\t\t%lf\n", res);
        res = dynamic_tournament(trace_root, 2);
        printf("Dynamic Tournament\t\tn=2\t\t%lf\n", res);
        res = dynamic_tournament(trace_root, 4);
        printf("Dynamic Tournament\t\tn=4\t\t%lf\n", res);
        res = dynamic_tournament(trace_root, 8);
        printf("Dynamic Tournament\t\tn=8\t\t%lf\n", res);

        /* cleanup */

        t = trace_root;
        trace_root = trace_root->next;
        free(t);

        while(trace_root) {
                t = trace_root;
                trace_root = trace_root->next;
                free(t);
        }

        return 0;
}
