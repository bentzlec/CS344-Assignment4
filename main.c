#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define SIZE 1000

#define NUM_LINES 50

//Buffers for Producer-Consumer Pipeline
char *buff1[NUM_LINES];
char *buff2[NUM_LINES];
char *buff3[NUM_LINES];

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

void put_buff(char * userInput) {
   pthread_mutex_lock(&mutex1);

   if(buff1[ProdIdx1] == NULL) {
      buff1[ProdIdx1] = (char*) malloc(SIZE * sizeof(char));
   }
   strcpy(buff1[ProdIdx1], userInput);
   
   ProdIdx1++;

   ctr1++;

   pthread_cond_signal(&full1);

   pthread_mutex_unlock(&mutex1);
}

char * get_buff_1() {
   pthread_mutex_lock(&mutex1);
   while(ctr1 == 0) {
      pthread_cond_wait(&full1, &mutex1);
   }
   size_t length = 0;
   char * input = strdup(buff1[ConIdx1]);
   ConIdx1++;
   ctr1--;
   pthread_mutex_unlock(&mutex1);

   return input;
}

void * lineSeparate() {
   char * input = get_buff_1();
   for(int i = 0; i < (strlen(input) - 1); i++) {
      if(input[i] == '\n') {
	 input[i] == ' ';
      }
   }
   printf("%s", input);
}

void * getUserInput() {
    size_t length;
    char * input = (char*) malloc(SIZE * sizeof(char));

    while(stop != 1) {
       getline(&input, &length, stdin); //Change to fgets

       if(strncmp(input, "STOP\n", 5) == 0) {
	  stop = 1;
       }else {
	  put_buff(input);
       }
    }

}

int main(int argc, char * argv[]) {
   pthread_t input_t, separate_t, plus_t, output_t;
   pthread_create(&input_t, NULL, getUserInput, NULL);

   //pthread_join(input_t, NULL);
   lineSeparate();
   return 0;
}
