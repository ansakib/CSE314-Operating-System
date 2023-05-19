/*
Inspired from Smoker-Ciggerate problem from "A little book of Semaphores" by Allen B. Downey

Problem Description: 

There are four processes in this problem: three sandwich-consumer processes and an agent process. 
Each of the consumer processes will make a sandwich and eat it. To make a sandwich requires bread, cheese, and meat.
Each consumer process has one of the three items. I.e., one process has bread, another has cheese, and a third has meat. 
The agent has an INFINITE supply of all three. 
The agent places two of the three items on the table, and the consumer that has the third item makes the sandwich. 
Synchronize the processes.

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_CONSUMERS 3
const char* items[] = {"bread", "cheese", "meat"};

pthread_mutex_t mlock;
pthread_cond_t agent;
pthread_cond_t consumer[NUM_CONSUMERS];

void* agent_thread(void* arg)
{
    int item1, item2;
    while(1){
        item1 = rand() % NUM_CONSUMERS;
        while(1){
            item2 = rand() % NUM_CONSUMERS;
            if(item1 != item2)
                break;
        }
        
        pthread_mutex_lock(&mlock);
        printf("Agent is placing %s(%d) and %s(%d) on the table\n", items[item1],item1, items[item2], item2);
        int consumer_to_wake = NUM_CONSUMERS - item1 - item2;
        pthread_cond_signal(&consumer[consumer_to_wake]);
        pthread_cond_wait(&agent, &mlock);
        pthread_mutex_unlock(&mlock);
    }
}

void* consumer_thread(void* arg)
{
    int consumer_no = *(int*)arg;
    //printf("Consumer %d", consumer_no);
    while(1)
    {
        
        
        //printf("abc");
        pthread_mutex_lock(&mlock);
        pthread_cond_wait(&consumer[consumer_no], &mlock);
        printf("Consumer %d is making sandwich\n", consumer_no);
        sleep(2);
        printf("Consumer %d is finished eating sandwich\n", consumer_no);
        pthread_cond_signal(&agent); //wake up agent
        pthread_mutex_unlock(&mlock);

    }
}

int main(int argc, char* argv[])
{
    pthread_t agent_tid;
    pthread_t consumer_tid[NUM_CONSUMERS];
    int consumer_no[NUM_CONSUMERS];

    for(int i=0; i<NUM_CONSUMERS; i++)
        printf("Consumer %d has %s(%d) item\n", i, items[i], i);
    printf("\n");

    pthread_mutex_init(&mlock, NULL);
    pthread_cond_init(&agent, NULL);
    for(int i=0; i<NUM_CONSUMERS; i++)
        pthread_cond_init(&consumer[i], NULL);
    
    
    
    for(int i=0; i<NUM_CONSUMERS; i++){
        consumer_no[i] = i;
        pthread_create(&consumer_tid[i], NULL, consumer_thread, &consumer_no[i]);
    }

    sleep(1);

    pthread_create(&agent_tid, NULL, agent_thread, NULL);

    pthread_join(agent_tid, NULL);

    for(int i=0; i<NUM_CONSUMERS; i++)
        pthread_join(consumer_tid[i], NULL);
    
    // pthread_mutex_destroy(&mlock);
    // pthread_cond_destroy(&agent);
    // for(int i=0; i<NUM_CONSUMERS; i++)
    //     pthread_cond_destroy(&consumer[i]);
}

