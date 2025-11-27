#include <iostream>
using namespace std;

class Process {
public:
    int id;
    int arrival;
    int burst;
    int remaining;
    int priority;
    int start;
    int finish;
};

void printGantt(int order[], int times[], int size) {
    cout << "\nGantt Chart:\n";
    for (int i = 0; i < size; i++) {
        cout << "|  P" << order[i] << "  ";
    }
    cout << "|\n";

    int current = 0;
    cout << 0;
    for (int i = 0; i < size; i++) {
        current = times[i];
        cout << "     " << current;
    }
    cout << "\n";
}

//  FCFS ALGORITHM
void fcfs(Process p[], int n) {
    int completionTimes[50], order[50], k = 0;
    int current = 0;

    // Sort by arrival time (simple bubble sort)
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (p[j].arrival > p[j+1].arrival) {
                Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (current < p[i].arrival)
            current = p[i].arrival;

        current += p[i].burst;

        order[k] = p[i].id;
        completionTimes[k] = current;
        k++;
    }

    printGantt(order, completionTimes, k);
}

//  SJF NON‑PREEMPTIVE
void sjf(Process p[], int n) {
    int completed = 0, current = 0;
    int order[50], timeMarks[50], k = 0;

    while (completed < n) {
        int idx = -1;
        int minBurst = 999999;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= current && p[i].remaining > 0) {
                if (p[i].burst < minBurst) {
                    minBurst = p[i].burst;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            current++;
            continue;
        }

        current += p[idx].burst;
        p[idx].remaining = 0;
        completed++;

        order[k] = p[idx].id;
        timeMarks[k] = current;
        k++;
    }

    printGantt(order, timeMarks, k);
}

//  SRJF – Shortest Remaining Time First
void srjf(Process p[], int n) {
    int completed = 0, current = 0;
    int order[500], timeMarks[500], k = 0;
    int lastProcess = -1;

    while (completed < n) {
        int idx = -1, minRemain = 999999;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= current && p[i].remaining > 0) {
                if (p[i].remaining < minRemain) {
                    minRemain = p[i].remaining;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            current++;
            continue;
        }

        // Log only when process changes
        if (idx != lastProcess) {
            order[k] = p[idx].id;
            timeMarks[k] = current;
            k++;
            lastProcess = idx;
        }

        p[idx].remaining--;
        current++;

        if (p[idx].remaining == 0)
            completed++;
    }

    // Close chart
    timeMarks[k] = current;

    printGantt(order, timeMarks, k);
}

// PRIORITY SCHEDULING (NON‑PREEMPTIVE)
void priorityScheduling(Process p[], int n) {
    int completed = 0, current = 0;
    int order[50], timeMarks[50], k = 0;

    while (completed < n) {
        int idx = -1, minPriority = 999999;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= current && p[i].remaining > 0) {
                if (p[i].priority < minPriority) {
                    minPriority = p[i].priority;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            current++;
            continue;
        }

        current += p[idx].burst;
        p[idx].remaining = 0;
        completed++;

        order[k] = p[idx].id;
        timeMarks[k] = current;
        k++;
    }

    printGantt(order, timeMarks, k);
}

//   ROUND ROBIN
void roundRobin(Process p[], int n, int quantum) {
    int completed = 0, current = 0;
    int order[200], timeMarks[200], k = 0;

    while (completed < n) {
        bool progress = false;

        for (int i = 0; i < n; i++) {
            if (p[i].remaining > 0 && p[i].arrival <= current) {
                progress = true;

                order[k] = p[i].id;

                if (p[i].remaining > quantum) {
                    current += quantum;
                    p[i].remaining -= quantum;
                } else {
                    current += p[i].remaining;
                    p[i].remaining = 0;
                    completed++;
                }

                timeMarks[k] = current;
                k++;
            }
        }

        if (!progress)
            current++;
    }

    printGantt(order, timeMarks, k);
}

//MAIN PROGRAM
int main() {
    int n, choice, quantum;

    cout << "Enter number of processes: ";
    cin >> n;

    Process p[50];

    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "\nProcess P" << p[i].id << ":\n";
        cout << "Arrival Time: ";
        cin >> p[i].arrival;
        cout << "Burst Time: ";
        cin >> p[i].burst;
        p[i].remaining = p[i].burst;
        cout << "Priority (if needed; put any number): ";
        cin >> p[i].priority;
    }

    cout << "\nChoose Scheduling Algorithm:\n";
    cout << "1. FCFS\n2. SJF\n3. SRJF\n4. Priority\n5. Round Robin\n";
    cin >> choice;

    if (choice == 1) fcfs(p, n);
    else if (choice == 2) sjf(p, n);
    else if (choice == 3) srjf(p, n);
    else if (choice == 4) priorityScheduling(p, n);
    else if (choice == 5) {
        cout << "Enter Quantum: ";
        cin >> quantum;
        roundRobin(p, n, quantum);
    }
    else cout << "Invalid choice.";

    return 0;
}