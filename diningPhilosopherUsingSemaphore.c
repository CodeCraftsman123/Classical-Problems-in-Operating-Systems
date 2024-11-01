#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<time.h>
#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
sem_t s,semaphores[N];
int states[N];

void thinking(int philNumber)
{
    states[philNumber] = THINKING;
    printf("\nPhilosopher %d is thinking.\n",philNumber);
    usleep(rand() % 2000000);
}

void test(int philNumber)
{
    if(states[philNumber]==HUNGRY && states[(philNumber+1)%N]!=EATING && states[(philNumber-1+N)%N]!=EATING )
    {
        states[philNumber] = EATING;
        sem_post(&semaphores[philNumber]);
    }
}

void takeForks(int philNumber)
{
    sem_wait(&s);
    states[philNumber] = HUNGRY;
    printf("\nPhilosopher %d is hungry.\n",philNumber);
    test(philNumber);
    sem_post(&s);
    sem_wait(&semaphores[philNumber]);
}

void eating(int philNumber)
{
    states[philNumber] = EATING;
    printf("\nPhilosopher %d is eating.\n",philNumber);
    usleep(rand() % 2000000);
}

void putForks(int philNumber)
{
    sem_wait(&s);
    states[philNumber] = THINKING;
    printf("\nPhilosopher %d has done eating.\n",philNumber);
    test((philNumber+1)%N);
    test((philNumber-1+N)%N);
    sem_post(&s);
}

void *philosopher(void *args)
{
    int philNumber = *(int *)args;
    thinking(philNumber);
    takeForks(philNumber);
    eating(philNumber);
    putForks(philNumber);
}

void main()
{
    srand(time(NULL));
    pthread_t threads[N];
    int philNumber[N];

    sem_init(&s,0,1);
    for(int i = 0 ; i < N ; i++)
    {
        philNumber[i] = i;
        sem_init(&semaphores[i],0,0);
    }

    for(int i = 0 ; i < N ; i++)
    {
        pthread_create(&threads[i],NULL,philosopher,&philNumber[i]);
    }

    for(int i = 0 ; i < N ; i++)
    {
        pthread_join(threads[i],NULL);
    }


    sem_destroy(&s);
    for(int i = 0 ; i < N ; i++)
        sem_destroy(&semaphores[i]);
}