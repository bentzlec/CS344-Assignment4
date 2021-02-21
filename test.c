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
/*
void put_buff(char * userInput) {
   pthread_mutex_lock(&mutex1);

   if(buff1[line] == NULL) {
      buff1[line] = (char*) malloc(SIZE * sizeof(char));
      Idx1 = Idx1 + (strlen(userInput) - 1);
      line++;
   }

   strcpy(buff1[ProdIdx1], userInput);
   
   ProdIdx1++;

   ctr1++;

   pthread_cond_signal(&full1);

   pthread_mutex_unlock(&mutex1);
}*/

int main() {
   /*size_t length;
   char * input = (char*) calloc(SIZE, sizeof(char));
   while(stop != 1) {
      getline(&input, &length, stdin);
      if(strncmp(input, "STOP\n", 5) == 0) {
	 stop = 1;
      }else {
	 put_buff(input);
      }
   }*/

   char * input;
   size_t length;

   getline(&input, &length, stdin);
   int len = strlen(input);
   printf("String length: %i\n", len);

   
    
   return 0;
}
