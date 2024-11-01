#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5  

int buffer[BUFFER_SIZE];  
int count = 0;            

sem_t empty;
sem_t full;   
sem_t s;  


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

int produceItem()
{
    return rand() % 100;
}

void addElementToBuffer(int item)
{
    buffer[count] = item;  
    printf("\nElement %d has been added to buffer\n",item);
    count++;
}

void* producer(void* arg) {
    while (1) {
        int item = produceItem();  
        printf("\nProducer is trying to access critical section\n");

        sem_wait(&empty); 
        sem_wait(&s);  

        printf("\nProducer has got access to critical section\n");
        addElementToBuffer(item);
        printf("\nBuffer after adding element:");
        displayBuffer();  

        printf("\nProducer has left the critical section\n");
        sem_post(&full);
        sem_post(&s);  

        usleep(rand() % 2000000);  
    }
}

int removeElementFromBuffer()
{
    count--;
    int item = buffer[count];  
    return item;
}

void consumeItem(int item)
{
    printf("\nElement %d has been consumed\n",item);
}


void* consumer(void* arg) {
    while (1) {
        printf("\nConsumer is trying to access critical section\n");
        sem_wait(&full);  
        sem_wait(&s);  

        printf("\nConsumer has got access to critical section\n");
        
        int item = removeElementFromBuffer();  
        printf("\nBuffer after removing element:");
        displayBuffer();  

        printf("\nConsumer has left the critical section\n");
        sem_post(&empty);  
        sem_post(&s);  
        consumeItem(item);

        usleep(rand() % 2000000);  
    }
}

int main() {
    srand(time(NULL));
    pthread_t prod_thread, cons_thread;


    sem_init(&empty, 0, BUFFER_SIZE);  
    sem_init(&full, 0, 0); 
    sem_init(&s, 0, 1);

    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Wait for threads to finish (optional, typically runs indefinitely)
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);


    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&s);

    return 0;
}