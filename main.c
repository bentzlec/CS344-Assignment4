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


void * getUserInput() {
   printf("Enter: ");
   fflush(stdout);
   scanf("%79s", &input);

   /* while(scanf("%79s", &input) != EOF) {
    * }
    * */

}

void put_buff(char * userInput) {
   pthread_mutex_lock(&mutex1);



}

int main(int argc, char * argv[]) {
   if(argc > 1) {
      for(int i = 0; i < argc; i++) {
	 if(strcmp(argv[i], "<") == 0) {


	 }else if(strcmp(argv[i], ">") == 0) {
	    FILE * fPtr1;
	    fPtr1 = fopen(argv[i+1], "w");
	    if(fPtr1 == NULL) {
	       return 1;
	    }
	    int num;
	    num = fileno(fPtr1);
	    dup2(num, STDOUT_FILENO);
	    dup2(num, STDERR_FILENO);
	    fclose(fPtr1);
	 }
      }
   }
   
   pthread_t input_t, separate_t, plus_t, output_t;
   pthread_create(&input_t, NULL, getUserInput, NULL);

   pthread_join(input_t, NULL);

   return 0;
}
