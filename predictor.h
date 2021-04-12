
#define MAX_TRACE_LINE_LEN 40 /* 2lazy2count */

struct trace {
        unsigned long addr;     /* assuming all the trace files are 64 bit addresses at max */
        int taken;              /* memory is cheap; native int performance is forever */
        struct trace *next;     /* linked list, seemed appropriate somehow */
};
typedef struct trace Trace;

double  static_taken(Trace *);
double  static_not_taken(Trace *);
double  dynamic_bm_gshare(Trace *, int);
double  dynamic_bm(Trace *, int);
double  dynamic_tournament(Trace *, int );