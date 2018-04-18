#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>


#define QUEUE_SIZE    2000
#define TOTAL_THREADS 500
#define ITERATIONS    1

clock_t start[TOTAL_THREADS];
clock_t end[TOTAL_THREADS];
double calctime[TOTAL_THREADS];
double lat[TOTAL_THREADS];

int s, j;
cpu_set_t cpuset;
pthread_t workers[TOTAL_THREADS];
typedef pthread_barrier_t *waiting_thread;
FILE *c1;
FILE *c2;
FILE *c3;
FILE *c4;


//struct to simulate fifo-based mutex

typedef struct
{
	int               init;
	waiting_thread    threads[QUEUE_SIZE];
	pthread_mutex_t   mutex, change_mutex;
	pthread_cond_t    queue_change;
	pthread_barrier_t queuing_sync;
	unsigned int      front, count, max;
} thread_queue;

static thread_queue main_queue = { 0 };

//END struct to simulate fifo-based mutex

//functions for dealing with queue

//initialize the queue
static int init_main_queue()
{
	if (main_queue.init) return 1;
	memset(main_queue.threads, 0, sizeof(waiting_thread) * QUEUE_SIZE);
	if (pthread_mutex_init(&main_queue.mutex, NULL) != 0)             return 0;
	if (pthread_mutex_init(&main_queue.change_mutex, NULL) != 0)      return 0;
	if (pthread_cond_init(&main_queue.queue_change, NULL) != 0)       return 0;
	if (pthread_barrier_init(&main_queue.queuing_sync, NULL, 2) != 0) return 0;
	main_queue.front = 0;
	main_queue.count = 0;
	main_queue.max   = QUEUE_SIZE;
	main_queue.init  = 1;
	return 1;
}


//clean up the queue
static int fini_main_queue()
{
	//(there should probably be some sort of 'main_queue.threads' cleanup)

	if (!main_queue.init) return 1;
	pthread_mutex_destroy(&main_queue.mutex);
	pthread_mutex_destroy(&main_queue.change_mutex);
	pthread_cond_destroy(&main_queue.queue_change);
	pthread_barrier_destroy(&main_queue.queuing_sync);
	return 1;
}


//push a thread onto the queue (from the thread itself)
static int push_waiting_thread(waiting_thread tThread)
{
	//obtain a queue lock
	if (!main_queue.init || pthread_mutex_lock(&main_queue.mutex) != 0) return 0;
	if (main_queue.count == main_queue.max)
	{
	pthread_mutex_unlock(&main_queue.mutex);
	return 0;
	}

	//notify the queuing thread of a change; the lock on the mutex will
	//prevent it from continuing until the change is made
	pthread_cond_broadcast(&main_queue.queue_change);

	//add the thread to the queue
	main_queue.threads[(main_queue.front + main_queue.count++) % main_queue.max] = tThread;
	pthread_mutex_unlock(&main_queue.mutex);

	return 1;
}


//wait for access to the "resource" (from the thread needing it)
static int wait_for_resource(waiting_thread tThread, int iIdentity)
{
	//add the thread to the queue and wait for its number to come up
	if (!push_waiting_thread(tThread)) return 0;
	fprintf(stderr, "thread %i waiting\n", iIdentity);
	int outcome = pthread_barrier_wait(tThread);
	return !outcome || outcome == PTHREAD_BARRIER_SERIAL_THREAD;
}


//"unlock" the simulated mutex when finished with it
static int finish_with_resource()
{
	if (!main_queue.init) return 0;
	int outcome = pthread_barrier_wait(&main_queue.queuing_sync);
	return !outcome || outcome == PTHREAD_BARRIER_SERIAL_THREAD;
}


//call the next thread in the queue (from the queuing thread)
static int pop_waiting_thread()
{
	int outcome;

	//obtain a queue lock
	if (!main_queue.init || pthread_mutex_lock(&main_queue.mutex) != 0) return 0;

	//if the queue is empty, block until a thread is added
	if (!main_queue.count)
	{
	pthread_mutex_unlock(&main_queue.mutex);
	//(there is a chance 'pthread_cond_broadcast' will happen between the
	//line above and the line below; therefore, a 'nanosleep' might be in
	//order here. a 'pthread_barrier_wait' wouldn't be any easier.)
	pthread_mutex_lock(&main_queue.change_mutex);
	pthread_cond_wait(&main_queue.queue_change, &main_queue.change_mutex);
	pthread_mutex_unlock(&main_queue.change_mutex);
	return 0;
	}

	waiting_thread next = main_queue.threads[main_queue.front++];
	--main_queue.count;
	main_queue.front %= main_queue.max;
	
	pthread_mutex_unlock(&main_queue.mutex);

	//continue the thread
	//(this will cause a segfault if 'next' has been destroyed already)
	outcome = pthread_barrier_wait(next);
	if (outcome && outcome != PTHREAD_BARRIER_SERIAL_THREAD) return 0;

	//wait for the thread to finish with the resource
	outcome = pthread_barrier_wait(&main_queue.queuing_sync);
	return !outcome || outcome == PTHREAD_BARRIER_SERIAL_THREAD;
}

//END functions for dealing with queue

//threads

//queuing thread
static void *queuing_thread(void *iIgnore)
{
	struct timespec delay = { 0, 100 * 1000 * 1000 };

	if (!main_queue.init) return NULL;
	fprintf(stderr, "queuing thread started\n");
	while (1)
	{
	pop_waiting_thread();
	nanosleep(&delay, NULL);
	}
	fprintf(stderr, "queuing thread finished\n");
	return NULL;
}


void perform_add(){
int a=1000,b=2000;
int sum=a+b;
}


void perform_mul(){
int a=10,b=20;
int prod=a*b;

}


void perform_pi_calc(){
int i;
double p = 0;
for ( i = 1; i <= 100000; i++) {
 if ( i % 2 == 0) {
 p = p - (4.0 / (2 * i - 1));
}
else {
 p = p + (4.0 / (2 * i - 1));
}
//printf( "%f\t", p );
}

}


//worker threads
static void *worker_thread(void *iIdentity)
{
	




	int identity = *(int*) iIdentity, iterations = ITERATIONS;
	clock_t temp,st,en;
   	

	if(lat[identity]==0){
   	temp=clock()-start[identity];   
   	lat[identity]=((double)temp)/CLOCKS_PER_SEC;
	}

	fprintf(stderr, "thread %i started\n", identity);

	pthread_barrier_t barrier;

	if (pthread_barrier_init(&barrier, NULL, 2) != 0) return NULL;

	while (iterations-- > 0 && wait_for_resource(&barrier, identity))
	{
	
	s = pthread_getaffinity_np(workers[identity], sizeof(cpu_set_t), &cpuset);
	for (j = 0; j < CPU_SETSIZE; j++)
               if (CPU_ISSET(j, &cpuset)){
                   printf("THREAD %d USING CPU %d\n",identity,j);
			if(j==0){
				st=clock();
				perform_add();
				en=clock()-st;
				fprintf(c1, "%d %f\n",identity, lat[identity]);
			}
			else if(j==1){
				st=clock();
				perform_mul();
				en=clock()-st;
				fprintf(c2, "%d %f\n",identity, lat[identity]);
			}
			else if(j==2){
				st=clock();
				perform_pi_calc();
				en=clock()-st;
				fprintf(c3, "%d %f\n",identity, lat[identity]);
			}
			else{
				fprintf(c4, "%d %f\n",identity, lat[identity]);
			}
			
		}
	fprintf(stderr, "thread %i has the resource \n", identity);
	finish_with_resource();
	fprintf(stderr, "thread %i is done with the resource \n", identity);
	}

	pthread_barrier_destroy(&barrier);
	calctime[identity]=((double)en)/CLOCKS_PER_SEC;
	printf("Calculation time for thread %d is %f\n",identity,calctime[identity]);
	fprintf(stderr, "thread %i finished\n\n\n", identity);

	return NULL;
}

//END threads


int main()
{
	pthread_t queueing;
	
	
	init_main_queue();
	
	c1 = fopen("cpu1.txt", "w");
	if (c1 == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	c2 = fopen("cpu2.txt", "w");
	if (c2 == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	c3 = fopen("cpu3.txt", "w");
	if (c3 == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	c4 = fopen("cpu4.txt", "w");
	if (c4 == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	int I;
	for (I = 0; I < TOTAL_THREADS; I++)
	{
	//create threads with a delay so they queue in order to start with
	start[I]=clock();
	
	pthread_create(workers + I, NULL, &worker_thread, &I);
	CPU_ZERO(&cpuset);
	CPU_SET((rand()%3), &cpuset);
	s = pthread_setaffinity_np(workers[I], sizeof(cpu_set_t), &cpuset);
	
	sleep(0.1);
	}

	pthread_create(&queueing, NULL, &queuing_thread, NULL);

	for (I = 0; I < TOTAL_THREADS; I++){
	pthread_join(workers[I], NULL);
	}

	pthread_cancel(queueing);
	pthread_join(queueing, NULL);

	fini_main_queue();

	for(int i=0;i<TOTAL_THREADS;i++){
	
	end[i]=clock();
	printf("Thread %d\n",i+1);
	printf("Creation time: %f\n",((double)start[i])/CLOCKS_PER_SEC);
	printf("End time: %f\n",((double)end[i])/CLOCKS_PER_SEC);
	printf("Latency %f\n\n\n",lat[i]);
   }
fclose(c1);
fclose(c2);
fclose(c3);
fclose(c4);

}
