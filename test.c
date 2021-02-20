#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define SIZE 1000

#define NUM_LINES 50

//Buffers for Producer-Consumer Pipeline
char buff1[NUM_LINES][SIZE] = {{0}};
char buff2[NUM_LINES][SIZE] = {{0}};
char buff3[NUM_LINES][SIZE] = {{0}};

char input[80];

//Number of items in each buffer
int ctr1, ctr2, ctr3 = 0;

//Producer indexes
int ProdIdx1, ProdIdx2, ProdIdx3 = 0;

//Consumer indexes
int ConIdx1, ConIdx2, ConIdx3 = 0;

//Mutex for each buffer
pthread_mutex_t mutex1, mutex2, mutex3 = PTHREAD_MUTEX_INITIALIZER;

//Condition variable for each buffer
pthread_cond_t full1, full2, full3 = PTHREAD_COND_INITIALIZER;

int stop = 0;

int main() {
   size_t length;
   char * input = (char*) calloc(SIZE, sizeof(char));
   while(stop != 1) {
      getline(&input, &length, stdin);
      if(strncmp(input, "STOP\n", 5) == 0) {
	 stop = 1;
	 //return 0;
      }else {
	 //put_buff(input);
      }
   }

   for(int i = 0; i < 


   
   
   
   
   
   
   
   return 0;
}
