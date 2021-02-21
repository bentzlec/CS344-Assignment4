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

int charNum = 0;

int line = 0;

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
   //printf("userInput: %s", userInput); 
   
   buff1[line] = malloc(strlen(userInput) + 1);
   strcpy(buff1[line], userInput);
   //printf("Buffer1 after strcpy: %s", buff1[line]);
   charNum = charNum + (strlen(userInput - 1));

   line++;
   printf("Lines counter in putt_buff: %i\n", line);
 
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
   char * input = malloc(strlen(buff1[ConIdx1]) + 1);
   strcpy(input, buff1[ConIdx1]);
   ConIdx1++;
   ctr1--;
   pthread_mutex_unlock(&mutex1);

   //printf("Before return: %s", input);
   printf("Lines: %i\n", line);
   return input;
}

void * lineSeparate() {
   char * input;
   
   printf("Lines2: %i\n", line);
   get_buff_1();
   
}

void * getUserInput() {
    size_t length = 0;
    char * input = NULL;

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
   pthread_create(&separate_t, NULL, lineSeparate, NULL);

   pthread_join(input_t, NULL);
   pthread_join(separate_t, NULL);
   return 0;
}
