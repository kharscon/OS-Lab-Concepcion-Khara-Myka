#include <stdio.h>

struct Process {
    int id, at, bt, ct, tat, wt;
};

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    int i, time = 0;
    double total_tat = 0, total_wt = 0;

    printf("Enter Arrival Time (AT):\n");
    for (i = 0; i < n; ++i) {
        scanf("%d", &p[i].at);
        p[i].id = i + 1;
    }
    printf("Enter Burst Time (BT):\n");
    for (i = 0; i < n; ++i)
        scanf("%d", &p[i].bt);

    for (i = 0; i < n; ++i) {
        if (time < p[i].at) time = p[i].at;
        p[i].ct = time + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        time = p[i].ct;
        total_tat += p[i].tat;
        total_wt += p[i].wt;
    }

    printf("ID\tAT\tBT\tCT\tTAT\tWT\n");
    for (i = 0; i < n; ++i)
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);

    printf("Average TAT = %.2lf\n", total_tat/n);
    printf("Average WT  = %.2lf\n", total_wt/n);

    printf("Gantt Chart:\n");
    for (i = 0; i < n; ++i)
        printf("| P%d ", p[i].id);
    printf("|\n0");
    time = 0;
    for (i = 0; i < n; ++i) {
        time = (time < p[i].at ? p[i].at : time) + p[i].bt;
        printf("%5d", time);
    }
    printf("\n");
    return 0;
}