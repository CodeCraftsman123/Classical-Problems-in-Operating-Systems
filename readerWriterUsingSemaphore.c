#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
int NUMBEROFREADERS;
int NUMBEROFWRITERS;

sem_t s;  
sem_t wrt;    
int readCount = 0;  
int sharedData = 80;

void readData(int readerId)
{
    usleep(rand() % 2000000);//Reading requires time.
    printf("\nRead Data:%d from Reader Number:%d\n", sharedData,readerId);
}

void *reader(void *arg) {
    int readerId = *(int *)arg;  
    printf("\nReader %d is trying to access critical region\n", readerId);

    sem_wait(&s);  
    readCount++;
    if (readCount == 1)
        sem_wait(&wrt);  
    sem_post(&s);  

    printf("\nReader %d has got access to critical section\n",readerId);
    readData(readerId);
   

    sem_wait(&s);
    readCount--;
    printf("\nReader %d has left the critical section\n",readerId);
    if (readCount == 0)
        sem_post(&wrt);  
    sem_post(&s);  

}

int produceData()
{
    return rand() %100;
}

void writeOnCriticalSection(int data,int writerId)
{
    sharedData = data;
    usleep(rand() % 2000000);//Writing requires time
    printf("\nWriter %d has wrote. Data Now:%d\n", writerId,sharedData);
}

// Writer function
void *writer(void *arg) {
    int writerId = *((int *)arg); 
    int dataToBeWritten = produceData();
    printf("\nWriter %d is trying to access critical region\n", writerId);

    sem_wait(&wrt);     
    printf("\nWriter %d has got access to critical region\n",writerId);
    writeOnCriticalSection(dataToBeWritten,writerId);
    printf("\nWriter %d has left the critical section\n",writerId);
    sem_post(&wrt);  

}

int main() {
    srand(time(NULL));
    printf("\nEnter total number Of Readers:");
    scanf("%d",&NUMBEROFREADERS);
    printf("\nEnter total number of Writers:");
    scanf("%d",&NUMBEROFWRITERS);

    pthread_t read[NUMBEROFREADERS], write[NUMBEROFWRITERS];
    int readerIds[NUMBEROFREADERS], writerIds[NUMBEROFWRITERS];

    sem_init(&s, 0, 1);  
    sem_init(&wrt, 0, 1);    

    for (int i = 0; i < NUMBEROFREADERS; i++) {
        readerIds[i] = i;
        pthread_create(&read[i], NULL, reader, &readerIds[i]);
    }

    for (int i = 0; i < NUMBEROFWRITERS; i++) {
        writerIds[i] = i;
        pthread_create(&write[i], NULL, writer, &writerIds[i]);
    }

    for (int i = 0; i < NUMBEROFREADERS; i++) {
        pthread_join(read[i], NULL);
    }
    for (int i = 0; i < NUMBEROFWRITERS; i++) {
        pthread_join(write[i], NULL);
    }

    sem_destroy(&s);
    sem_destroy(&wrt);

    return 0;
}