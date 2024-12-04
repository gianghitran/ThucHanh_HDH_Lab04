#include <stdio.h> 
#include <stdlib.h> 
#define SORT_BY_ARRIVAL 0 
#define SORT_BY_PID 1 
#define SORT_BY_BURST 2

#define SORT_BY_START 3 
 
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
//hàm time()
extern long time(long *t);


typedef struct{ 
    int iPID; 
    int iArrival, iBurst; 
    int iStart, iFinish, iWaiting, iResponse, iTaT; 
    int RemainBurst;

} PCB; 
 
void inputProcess(int n, PCB P[]) {
        srand(time(NULL)); 
    //n la so luong process
    for (int i =0; i <n; i++){
        // scanf("%d %d %d", &P[i].iPID, &P[i].iArrival, &P[i].iBurst ) ;
        P[i].iPID = i + 1;
        P[i].iArrival = rand() % 21; 
        P[i].iBurst = 2 + rand() % 11; 
        P[i].RemainBurst = P[i].iBurst;
    }
    }

 
void printProcess(int n, PCB P[]){
    printf(" --------------------===== RR Scheduling =====---------------------\n");
    // printf("--------------------------------------------------------------------\n");
    printf("│ PID │ Arrival │ Burst │ Start │ Finish │ Waiting│ Response│  TaT  │\n");
    printf(" -------------------------------------------------------------------\n");

    // In thông tin từng quá trình
    for (int i = 0; i < n; i++) {
        printf("│ %3d │ %7d │ %5d │ %5d │ %6d │ %6d │ %7d │ %5d │\n",
               P[i].iPID, P[i].iArrival, P[i].iBurst,
               P[i].iStart, P[i].iFinish, P[i].iWaiting,
               P[i].iResponse, P[i].iTaT);
    }
    printf(" -------------------------------------------------------------------\n");


}
void exportGanttChart(int n, PCB P[]) {
        for (int i =0; i<n; i++){
        printf("| %d ", P[i].iStart);
        for (int j=0; j < (P[i].iFinish-P[i].iStart);j++) printf("-");
        
        printf(" %d | %d ", P[i].iFinish, P[i].iPID); 
    }    
    printf("\n");
}

void pushProcess(int *n, PCB P[], PCB Q) {
    P[*n] = Q;
    (*n)++;
}

void removeProcess(int *n, int index, PCB P[]) {
    for (int i = index; i < *n - 1; i++) {
        P[i] = P[i + 1];
    }
    (*n)--;
}
int swapProcess(PCB *P, PCB *Q) {
    PCB temp = *P;
    *P = *Q;
    *Q = temp;
    return 0;
}
int partition (PCB P[], int low, int high, int iCriteria){
    int pivot;
    switch (iCriteria)
    {
        case 0:
            pivot = P[high].iArrival; break;
        case 1:
            pivot = P[high].iPID; break;
        case 2:
            pivot = P[high].iBurst; break;
        case 3:
            pivot = P[high].iStart; break;  
    }
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) 
    {
        int tmp;
        switch (iCriteria)
        {
            case 0:
                tmp = P[j].iArrival; break;
            case 1:
                tmp = P[j].iPID; break;
            case 2:
                tmp = P[j].iBurst; break;
            case 3:
                tmp = P[j].iStart; break;  
        }
        if (tmp < pivot) 
        {
            i++;
            swapProcess(&P[i], &P[j]);
        }
    }
    swapProcess(&P[i + 1], &P[high]);  
    return i + 1;
}
void quickSort(PCB P[], int low, int high, int iCriteria){
    
     if (low < high) 
    {
        int pi = partition(P, low, high,iCriteria);
        quickSort(P, low, pi - 1,iCriteria);
        quickSort(P, pi + 1, high,iCriteria);
    }
}

void calculateAWT(int n, PCB P[]) {
     float time=0;
    for(int i =0; i<n;i++){
        time += (P[i].iFinish - P[i].iArrival-P[i].iBurst);    
    }
    time /= n;
    printf("Average waiting time = %f\n", time);
}
void calculateATaT(int n, PCB P[]) {
    float time=0;
    for(int i =0; i<n;i++){
        time += (P[i].iFinish - P[i].iArrival);    
    }
    time /= n;
    printf("Average turn around time = %f\n", time);
}
int main() 
{ 
    PCB Input[10]; 
    PCB ReadyQueue[10]; 
    PCB TerminatedArray[10]; 
    PCB GanttChart[100];

    int currentTime;
    int quantum;
    printf("Please input Quantum time: ");
    scanf("%d", &quantum);

    int iNumberOfProcess; 
    printf("Please input number of Process: "); 
    scanf("%d", &iNumberOfProcess); 
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0, iGantt = 0;
 
    inputProcess(iNumberOfProcess, Input); 
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL); 
 
    pushProcess(&iReady, ReadyQueue, Input[0]); 
    removeProcess(&iRemain, 0, Input); 
 
    ReadyQueue[0].iStart = ReadyQueue[0].iArrival; 
    int minTime = MIN(quantum, ReadyQueue[0].RemainBurst);
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + minTime;
    currentTime = minTime;
    ReadyQueue[0].RemainBurst -= minTime;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival; 
    ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - ReadyQueue[0].iBurst; 
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival; 

    printf("\nReady Queue: \n"); 
    printProcess(1, ReadyQueue); 
   while (iTerminated < iNumberOfProcess) 
{ 
    while (iRemain > 0) 
    { 
        if (Input[0].iArrival <= ReadyQueue[0].iFinish) 
        { 
            pushProcess(&iReady, ReadyQueue, Input[0]); 
            removeProcess(&iRemain, 0, Input);   
        } 
        else if (iReady == 1) 
        {
            pushProcess(&iReady, ReadyQueue, Input[0]); 
            removeProcess(&iRemain, 0, Input);   
            break; 
        }
        else 
        {
            break; 
        }
    } 
  
    if (iReady > 0) 
    { 
        while ((iReady > 1 && (ReadyQueue[0].iFinish < ReadyQueue[1].iArrival || iReady == 1)) && ReadyQueue[0].RemainBurst > 0) 
        {   
            int minTime = MIN(quantum, ReadyQueue[0].RemainBurst);
            ReadyQueue[0].iFinish += minTime;
            currentTime += minTime;
            ReadyQueue[0].RemainBurst -= minTime;
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival; 
            ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - ReadyQueue[0].iBurst; 
        }

        if (ReadyQueue[0].RemainBurst > 0)
        {
            pushProcess(&iReady, ReadyQueue, ReadyQueue[0]);
        }
        else 
        {
            pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]); 
        }

        pushProcess(&iGantt, GanttChart, ReadyQueue[0]); 
        removeProcess(&iReady, 0, ReadyQueue); 
        
        ReadyQueue[0].iStart = (ReadyQueue[0].iArrival <= GanttChart[iGantt - 1].iFinish) ?
        GanttChart[iGantt - 1].iFinish : ReadyQueue[0].iArrival; 
        currentTime = (ReadyQueue[0].iArrival <= GanttChart[iGantt - 1].iFinish) ?
        currentTime : ReadyQueue[0].iStart;

        int minFinish = MIN(currentTime + ReadyQueue[0].RemainBurst, currentTime + quantum); 
        ReadyQueue[0].iFinish = minFinish;
        int timeUsed = MIN(quantum, ReadyQueue[0].RemainBurst);
        currentTime += timeUsed;
        ReadyQueue[0].RemainBurst -= timeUsed;

        ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival; 
        ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival; 
        ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - ReadyQueue[0].iBurst; 
    } 
}
 
    printf("\n===== RR Scheduling =====\n"); 
    printProcess(iTerminated,TerminatedArray);

    printf("\n===== GANTT Chart =====\n");
    exportGanttChart(iGantt, GanttChart); 
    
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID); 
 
    calculateAWT(iTerminated, TerminatedArray); 
    calculateATaT(iTerminated, TerminatedArray); 
     
    return 0; 
} 