/*
  proconssumer.cpp
  An example of using SDL threads and semaphores.
  The producer-consumer problem using semaphores.
  Compile:  g++ -o proconsumer proconsumer.cpp -lSDL -lpthread
  Execute:  ./proconsumer
*/

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define	N	5			//number of slots in buffer
SDL_sem	*mutex;				//controls access to critical region
SDL_sem *nempty;			//counts number of empty slots
SDL_sem *nfilled;			//counts number of filled slots
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
int producer ( void *data )
{
  int item;
  char *tname = ( char *) data;	//thread name
  while ( true ) {
    SDL_Delay ( rand() % 1000 );//random delay
    printf("\n%s : ", tname );
    item = produce_item();	//produce an item
    SDL_SemWait ( nempty );	//decrement empty count
    SDL_SemWait ( mutex );	//entering critical section
    insert_item ( item );
    SDL_SemPost ( mutex );	//leave Critical Section
    SDL_SemPost ( nfilled );	//increment nfilled 
  }

  return 0;
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
int consumer ( void *data )
{
  int item;
  char *tname = ( char * ) data;//thread name

  while ( true ) {
    SDL_Delay ( rand() % 1000 );//random delay
    SDL_SemWait ( nfilled );	//decrement filled count
    SDL_SemWait ( mutex );	//entering critical section
    printf("\n%s : ", tname );
    item = remove_item ();	//take item from buffer
    SDL_SemPost ( mutex );	//leave Critical Section
    SDL_SemPost ( nempty );	//increment count of empty slots
    //can do something with item here
  }
}

int main ()
{
  SDL_Thread *id1, *id2, *id3;          //thread identifiers
  char *tnames[3] = { "Producer", "Consumer" }; //names of threads

  mutex = SDL_CreateSemaphore ( 1 ); 	//initialize mutex to 1
  nempty = SDL_CreateSemaphore ( N );	//initially all slots empty
  nfilled  = SDL_CreateSemaphore ( 0 );	//no slot filled
  id1 = SDL_CreateThread ( producer, tnames[0] );
  id2 = SDL_CreateThread ( consumer, tnames[1] );

  //wait for the threads to exit
  SDL_WaitThread ( id1, NULL );
  SDL_WaitThread ( id2, NULL );

  return 0;
}
