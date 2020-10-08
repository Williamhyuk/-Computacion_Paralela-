#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define thread_count	5

long long sum = 0;
long long n = pow(10,8);
//long long n = 1000;
long long flag = 0;
long long count = 0;
pthread_mutex_t mutex;

void *Brrier_with_busy_waiting_and_mutex(void* my_number);
void *Thread_sum(void* rank);

int main(){

  clock_t t_ini, t_fin;
  double secs;

  t_ini = clock();
  /* ...hacer algo... */
 
 
  pthread_t* thread_handles;
  long thread;
  thread_handles = malloc(thread_count*sizeof(pthread_t));
  //pthread_mutex_init(&mutex, NULL);
  for (thread = 0; thread < thread_count; thread++)
    pthread_create(&thread_handles[thread], NULL, Thread_sum, (void*) thread); 
  
  for (thread = 0; thread < thread_count; thread++)
    pthread_join(thread_handles[thread], NULL);
  //*******printf("sum = %lld\n", sum);
  //pthread_mutex_destroy(&mutex);
  free(thread_handles);

  t_fin = clock();

  secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
  printf("%dthread_count\n",thread_count);
  printf("%f segundos\n", secs);
  //printf("%.16g milisegundos\n", secs * 1000.0);
  return 0;
}

void *Brrier_with_busy_waiting_and_mutex(void* my_number){
  pthread_mutex_lock(&mutex);
  count++;
  sum += (long)my_number;
  pthread_mutex_unlock(&mutex);
  printf("I am waiting for my coworker\n");
  while(count < thread_count)
    ; //Do nothing. Just keep iterating
  printf("I am NOT waiting anymore\n");
  return NULL;
}

void *Thread_sum(void* rank) {
  long my_rank = (long) rank;
  double factor;
  long long i;
  long long my_n = n/thread_count;
  long long my_first_i = my_n*my_rank;
  long long my_last_i = my_first_i + my_n;
  
  if (my_first_i % 2 == 0)
    factor = 1.0;
  else
    factor = -1.0;

  for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
    while (flag != my_rank);
    sum += factor/(2*i+1);
    //printf("sum= %lld\n",sum);
    flag = (flag+1) % thread_count;
    count++;
  }

  return NULL;
} /* Thread sum */