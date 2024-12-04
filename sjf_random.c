
#include <stdio.h>
#include <stdlib.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3
//hàm time()
extern long time(long *t);

typedef struct{
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;
void inputProcess(int n, PCB P[])
{
    srand(time(NULL)); 
    //n la so luong process
    for (int i =0; i <n; i++){
        // scanf("%d %d %d", &P[i].iPID, &P[i].iArrival, &P[i].iBurst ) ;
        P[i].iPID = i + 1;
        P[i].iArrival = rand() % 21; 
        P[i].iBurst = 2 + rand() % 11; 
    }
}
void printProcess(int n, PCB P[]){
    printf(" --------------------===== SJF Scheduling =====---------------------\n");
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
void exportGanttChart (int n, PCB P[]){
    for (int i =0; i<n; i++){
        printf("| %d ", P[i].iStart);
        for (int j=0; j < (P[i].iFinish-P[i].iStart);j++) printf("-");
        
        printf(" %d | %d ", P[i].iFinish, P[i].iPID); 
    }    
    printf("\n");
}

void pushProcess(int *n, PCB P[], PCB Q){
    P[*n] = Q;
    (*n)++;
}
void removeProcess(int *n, int index, PCB P[]){
    for (int i = index; i < *n-1; i++){
        P[i]=P[i+1];
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
void calculateAWT(int n, PCB P[]){
    float time=0;
    for(int i =0; i<n;i++){
        time += (P[i].iFinish - P[i].iArrival-P[i].iBurst);    
    }
    time /= n;
    printf("Average waiting time = %f\n", time);
}
void calculateATaT(int n, PCB P[]){
    float time=0;
    for(int i =0; i<n;i++){
        time += (P[i].iFinish - P[i].iArrival);    
    }
    time /= n;
    printf("Average turn around time = %f\n", time);
}
int main()
{
    PCB Input[10]; //Mang luu cac tien trinh nhap vao
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    int iNumberOfProcess;
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
    //Sort tien trinh theo arrival time
    inputProcess(iNumberOfProcess, Input);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    //Phai push tat ca tien trinh
    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);
    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
    printf("\nReady Queue: \n");
    printProcess(1, ReadyQueue);
    
    int currentTime = 0; // Thời gian hiện tại của hệ thống

    while (iTerminated < iNumberOfProcess) {
    // Chuyển tiến trình từ Input sang ReadyQueue nếu đã đến
    while (iRemain > 0 && Input[0].iArrival <= currentTime) {
        pushProcess(&iReady, ReadyQueue, Input[0]);
        quickSort(ReadyQueue, 0, iReady - 1, SORT_BY_BURST); // Sắp xếp theo Burst
        removeProcess(&iRemain, 0, Input);
    }

    if (iReady == 0) {
        // CPU rảnh, cập nhật thời gian đến tiến trình tiếp theo
        currentTime = Input[0].iArrival;
        pushProcess(&iReady, ReadyQueue, Input[0]);
        removeProcess(&iRemain, 0, Input);
        ReadyQueue[0].iStart = currentTime;
        ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
        ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
        ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
        ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
    } else if (iReady > 0){
        // Chọn tiến trình đầu tiên từ ReadyQueue
        if (currentTime > ReadyQueue[0].iArrival)
            ReadyQueue[0].iStart = currentTime;
        else 
            ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
        ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
        ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
        ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
        ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;

        // Đưa tiến trình vào TerminatedArray
        pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
        currentTime = ReadyQueue[0].iFinish;
        removeProcess(&iReady, 0, ReadyQueue);
    }
}
    printf("\n===== SJF Scheduling =====\n");
    printProcess(iTerminated,TerminatedArray);

    printf("\n===== GANTT Chart =====\n");

    exportGanttChart(iTerminated, TerminatedArray);
    
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    calculateAWT(iTerminated, TerminatedArray);//Thoi gian doi trung binh
    calculateATaT(iTerminated, TerminatedArray);//Thoi gian hoan thanh trung binh

    return 0;
}
