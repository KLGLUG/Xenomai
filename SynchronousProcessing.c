#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<time.h>

void *functionC(void* k);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;
clock_t start[10];
clock_t end[10];
double lat[10];

int main()
{
   int rc1, rc2,i=0;
   pthread_t threads[10];


   for(i=0;i<10;++i){
	start[i]=clock();
   	if( (rc1=pthread_create( &threads[i], NULL, &functionC, (void *)&i)) )
   	{
      	printf("Thread creation failed: %d\n", rc1);
  	}
	pthread_join( threads[i], NULL);
	end[i]=clock();
   }
 

   for(i=0;i<10;i++){
	printf("Thread %d\n",i+1);
	printf("Start time: %f\n",((double)start[i])/CLOCKS_PER_SEC);
	printf("End time: %f\n",((double)end[i])/CLOCKS_PER_SEC);
	printf("Latency %f\n\n\n",lat[i]);
   }
   exit(EXIT_SUCCESS);
}

void *functionC(void *k)
{
    pthread_mutex_lock( &mutex1 );
   clock_t temp;
   int *th=(int *)k;
   int temp2=*th;


   temp=clock()-start[temp2];
   
   lat[temp2]=((double)temp)/CLOCKS_PER_SEC;
  
   pthread_mutex_unlock( &mutex1 );
}
