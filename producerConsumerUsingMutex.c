#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5  

int buffer[BUFFER_SIZE];  
int count = 0;            

pthread_mutex_t mutex;  // For mutual exclusion
pthread_cond_t bufferNotFull;  // Condition variable to check if the buffer is not full
pthread_cond_t bufferNotEmpty;  // Condition variable to check if the buffer is not empty

void displayBuffer() {
    printf("[ ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i < count)
            printf("%d ", buffer[i]);
        else
            printf("- "); 
    }
    printf("] ");
    
    if (count == 0) {
        printf("(Buffer is empty)\n");
    } else if (count == BUFFER_SIZE) {
        printf("(Buffer is full)\n");
    } else {
        printf("\n");
    }
}

int produceItem() {
    return rand() % 100;
}

void addElementToBuffer(int item) {
    buffer[count] = item;  
    printf("\nElement %d has been added to buffer\n", item);
    count++;
}

void* producer(void* arg) {
    while (1) {
        int item = produceItem();  
        printf("\nProducer is trying to access critical section\n");

        pthread_mutex_lock(&mutex);  // Lock mutex to enter critical section
        
        // Wait until buffer is full
        while (count == BUFFER_SIZE) {
            printf("\nProducer waiting, buffer is full\n");
            pthread_cond_wait(&bufferNotFull, &mutex);
        }   

        printf("\nProducer has got access to critical section\n");
        addElementToBuffer(item);
        printf("\nBuffer after adding element:");
        displayBuffer();  

        printf("\nProducer has left the critical section\n");
        pthread_cond_signal(&bufferNotEmpty);  // Signal consumer that buffer is not empty
        pthread_mutex_unlock(&mutex);  // Unlock mutex after leaving critical section

        usleep(rand() % 2000000);  // Simulate time taken to produce an item
    }
}

int removeElementFromBuffer() {
    count--;
    int item = buffer[count];  
    return item;
}

void consumeItem(int item) {
    printf("\nElement %d has been consumed\n", item);
}

void* consumer(void* arg) {
    while (1) {
        printf("\nConsumer is trying to access critical section\n");

        pthread_mutex_lock(&mutex);  // Lock mutex to enter critical section
        
        // Wait until buffer is not empty
        while (count == 0) {
            printf("\nConsumer waiting, buffer is empty\n");
            pthread_cond_wait(&bufferNotEmpty, &mutex);
        }

        printf("\nConsumer has got access to critical section\n");
        
        int item = removeElementFromBuffer();  
        printf("\nBuffer after removing element:");
        displayBuffer();  

        printf("\nConsumer has left the critical section\n");
        pthread_cond_signal(&bufferNotFull);  
        pthread_mutex_unlock(&mutex); 
        
        consumeItem(item);

        usleep(rand() % 2000000);  
    }
}

int main() {
    srand(time(NULL));
    pthread_t prod_thread, cons_thread;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&bufferNotFull, NULL);
    pthread_cond_init(&bufferNotEmpty, NULL);

    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&bufferNotFull);
    pthread_cond_destroy(&bufferNotEmpty);

    return 0;
}