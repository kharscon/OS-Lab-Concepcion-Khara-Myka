#include <stdio.h>
#include <stdbool.h>

struct Process {
    int id, at, bt, ct, tat, wt;
    bool done;
};

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    int i, completed = 0, time = 0;

    printf("Enter Arrival Time (AT):\n");
    for (i = 0; i < n; ++i) {
        scanf("%d", &p[i].at);
        p[i].id = i + 1;
        p[i].done = false;
    }
    printf("Enter Burst Time (BT):\n");
    for (i = 0; i < n; ++i)
        scanf("%d", &p[i].bt);

    double total_tat = 0, total_wt = 0;
    int gantt[n], gcount = 0;

    while (completed < n) {
        int idx = -1, min_bt = 1000000000;
        for (i = 0; i < n; ++i) {
            if (!p[i].done && p[i].at <= time && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                idx = i;
            }
        }
        if (idx == -1) { time++; continue; }
        gantt[gcount++] = p[idx].id;
        time += p[idx].bt;
        p[idx].ct = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        total_tat += p[idx].tat;
        total_wt += p[idx].wt;
        p[idx].done = true;
        completed++;
    }

    printf("ID\tAT\tBT\tCT\tTAT\tWT\n");
    for (i = 0; i < n; ++i) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("Average TAT = %.2lf\n", total_tat/n);
    printf("Average WT  = %.2lf\n", total_wt/n);

    printf("Gantt Chart:\n");
    for (i = 0; i < gcount; ++i)
        printf("| P%d ", gantt[i]);
    printf("|\n0");
    time = 0;
    for (i = 0; i < gcount; ++i) {
        int j;
        for (j = 0; j < n; ++j) {
            if (p[j].id == gantt[i]) {
                time += p[j].bt;
                printf("%5d", time);
                break;
            }
        }
    }
    printf("\n");
    return 0;
}