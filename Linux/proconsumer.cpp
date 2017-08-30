/*
  proconssumer.cpp
  An example of using posix threads and semaphores.
  The producer-consumer problem using semaphores.
  Compile:  g++ -o proconsumer proconsumer.cpp -lpthread
  Execute:  ./proconsumer
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <semaphore.h>

using namespace std;

#define	N	5			    //number of slots in buffer
sem_t	mutex;		//controls access to critical region
sem_t   nempty;		//counts number of empty slots
sem_t   nfilled;	    //counts number of filled slots
int buffer[N];

int produce_item()
{
  int n = rand() % 1000;	
  return n;
}

void insert_item ( int item )
{
  static int tail = 0;
  
  buffer[tail] = item;
  printf("insert %d at %d", item, tail ); 
  tail = ( tail + 1 ) % N; 
}

//a producer thread
void* producer ( void *data )
{
  int item;
  char *tname = ( char *) data;	//thread name
  while ( true ) {
    usleep ( rand() % 1000 );//random delay
    printf("\n%s : ", tname );
    item = produce_item();	//produce an item
    sem_wait ( &nempty );	//decrement empty count
    sem_wait ( &mutex );	//entering critical section
    insert_item ( item );
    sem_post ( &mutex );	//leave Critical Section
    sem_post ( &nfilled );  //increment nfilled 
  }
}

int remove_item ()
{
  static int head = 0;
  int item;

  item = buffer[head];
  printf("remove %d at %d", item, head );
  head = ( head + 1 ) % N;
  return item;
}

//a consumer thread 
void* consumer ( void *data )
{
  int item;
  char *tname = ( char * ) data;//thread name

  while ( true ) {
    usleep ( rand() % 1000 );//random delay
    sem_wait ( &nfilled );	//decrement filled count
    sem_wait ( &mutex );	    //entering critical section
    printf("\n%s : ", tname );
    item = remove_item ();	//take item from buffer
    sem_post ( &mutex );	    //leave Critical Section
    sem_post ( &nempty );	//increment count of empty slots
    //can do something with item here
  }
}

int main ()
{
  int res;
  pthread_t id1, id2;          //thread identifiers
  void *thread_result1, *thread_result2;
  char *tnames[3] = { "Producer", "Consumer" }; //names of threads

  res = sem_init ( &mutex, 0, 1 ); 	//initialize mutex to 1
  res = sem_init ( &nempty, 0, N );	//initially all slots empty
  res = sem_init ( &nfilled, 0, 0 );	//no slot filled
  res = pthread_create ( &id1, NULL, producer, tnames[0] );
  res = pthread_create ( &id2, NULL, consumer, tnames[1] );

  //wait for the threads to exit
  pthread_join ( id1, &thread_result1 );
  pthread_join ( id2, &thread_result2 );

  sem_destroy(&mutex);
  sem_destroy(&nempty);
  sem_destroy(&nfilled);
  
  return 0;
}
