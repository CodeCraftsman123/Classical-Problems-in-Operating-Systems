#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int NUMBEROFREADERS;
int NUMBEROFWRITERS;

pthread_mutex_t mutex;   
pthread_mutex_t writeLock; 
int readCount = 0;   
int sharedData = 80; 

void readData(int readerId) {
    usleep(rand() % 2000000); // Simulate time for reading
    printf("\nReader %d read data: %d\n", readerId, sharedData);
}

void *reader(void *arg) {
    int readerId = *(int *)arg;
    printf("\nReader %d is trying to access critical region\n", readerId);

    pthread_mutex_lock(&mutex);
    readCount++;
    if (readCount == 1)
        pthread_mutex_lock(&writeLock);
    pthread_mutex_unlock(&mutex);

    printf("\nReader %d has got access to critical section\n", readerId);
    readData(readerId);

    pthread_mutex_lock(&mutex);
    readCount--;
    printf("\nReader %d has left the critical section\n", readerId);
    if (readCount == 0)
        pthread_mutex_unlock(&writeLock);
    pthread_mutex_unlock(&mutex);

}

int produceData() {
    return rand() % 100;
}

void writeOnCriticalSection(int data, int writerId) {
    sharedData = data;
    usleep(rand() % 2000000); // Simulate time for writing
    printf("\nWriter %d wrote data: %d\n", writerId, sharedData);
}

// Writer function
void *writer(void *arg) {
    int writerId = *(int *)arg;
    int dataToBeWritten = produceData();
    printf("\nWriter %d is trying to access critical region\n", writerId);

    pthread_mutex_lock(&writeLock);
    printf("\nWriter %d has got access to critical region\n", writerId);

    writeOnCriticalSection(dataToBeWritten, writerId);

    printf("\nWriter %d has left the critical section\n", writerId);
    pthread_mutex_unlock(&writeLock); 
}

int main() {
    printf("\nEnter total Number Of Readers:");
    scanf("%d",&NUMBEROFREADERS);
    printf("\nEnter total Number Of Writers:");
    scanf("%d",&NUMBEROFWRITERS);
    srand(time(NULL));

    pthread_t read[NUMBEROFREADERS], write[NUMBEROFWRITERS];  // 5 readers and 2 writers
    int readerIds[NUMBEROFREADERS], writerIds[NUMBEROFWRITERS];

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&writeLock, NULL);

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

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&writeLock);

    return 0;
}