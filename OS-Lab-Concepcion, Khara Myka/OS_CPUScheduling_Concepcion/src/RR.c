

#include <stdio.h>
#include <stdlib.h>

#define QUANTUM 2

typedef struct {
    int id;
    int at; /* arrival time */
    int bt; /* burst time */
    int rem; /* remaining time */
    int ct; /* completion time */
    int tat; /* turnaround time */
    int wt;  /* waiting time */
} Process;

int main(void) {
    int n;
    if (printf("Enter number of processes: ") < 0) return 1;
    if (scanf("%d", &n) != 1 || n <= 0) { fprintf(stderr, "Invalid number of processes\n"); return 1; }

    Process *p = malloc(sizeof(Process) * n);
    if (!p) { perror("malloc"); return 1; }

    printf("Enter Arrival Time (AT):\n");
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &p[i].at) != 1) { fprintf(stderr, "Bad input\n"); free(p); return 1; }
        p[i].id = i + 1;
    }
    printf("Enter Burst Time (BT):\n");
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &p[i].bt) != 1) { fprintf(stderr, "Bad input\n"); free(p); return 1; }
        p[i].rem = p[i].bt;
        p[i].ct = p[i].tat = p[i].wt = 0;
    }

    /* Ready queue: use simple circular buffer */
    int capacity = 10000;
    int *queue = malloc(sizeof(int) * capacity);
    if (!queue) { perror("malloc"); free(p); return 1; }
    int head = 0, tail = 0;

    int time = 0;
    int completed = 0;
    int idx = 0; /* next to arrive */

    /* Gantt chart records */
    int *g_pid = malloc(sizeof(int) * capacity);
    int *g_time = malloc(sizeof(int) * capacity);
    if (!g_pid || !g_time) { perror("malloc"); free(p); free(queue); free(g_pid); free(g_time); return 1; }
    int g_len = 0;

    /* enqueue arrivals at time 0 */
    while (idx < n && p[idx].at <= time) { queue[tail++] = idx; if (tail == capacity) tail = 0; idx++; }

    while (completed < n) {
        if (head != tail) {
            int cur = queue[head++]; if (head == capacity) head = 0;
            int exec = (p[cur].rem <= QUANTUM) ? p[cur].rem : QUANTUM;
            time += exec;
            p[cur].rem -= exec;
            g_pid[g_len] = p[cur].id;
            g_time[g_len] = time;
            g_len++;

            /* enqueue processes that arrived during this quantum */
            while (idx < n && p[idx].at <= time) { queue[tail++] = idx; if (tail == capacity) tail = 0; idx++; }

            if (p[cur].rem > 0) { queue[tail++] = cur; if (tail == capacity) tail = 0; }
            else { p[cur].ct = time; p[cur].tat = p[cur].ct - p[cur].at; p[cur].wt = p[cur].tat - p[cur].bt; completed++; }
        } else {
            /* no ready process; jump to next arrival */
            if (idx < n) {
                time = p[idx].at;
                queue[tail++] = idx; if (tail == capacity) tail = 0; idx++;
            }
        }
    }

    double total_tat = 0.0, total_wt = 0.0;
    printf("ID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; ++i) {
        total_tat += p[i].tat; total_wt += p[i].wt;
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("Average TAT = %.2f\n", total_tat / n);
    printf("Average WT  = %.2f\n", total_wt / n);

    printf("Gantt Chart:\n");
    for (int i = 0; i < g_len; ++i) printf("| P%d ", g_pid[i]);
    printf("|\n0");
    for (int i = 0; i < g_len; ++i) printf("%5d", g_time[i]);
    printf("\n");

    free(p); free(queue); free(g_pid); free(g_time);
    return 0;
}
