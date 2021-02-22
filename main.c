#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define SIZE 1000
#define NUM_LINES 50

char buff1[NUM_LINES][SIZE] = {{0}};
char buff2[NUM_LINES][SIZE] = {{0}};
char buff3[NUM_LINES][SIZE] = {{0}};

int line1, line2, line3 = 0; //How many lines in each buffer, basically an item counter for each buffer

int con_idx1, con_idx2, con_idx3 = 0; //Consumer index

int stop = 0; //Flag for when "STOP\n" is read

pthread_mutex_t mutex_1, mutex_2, mutex_3 = PTHREAD_MUTEX_INITIALIZER; //Took these names from the example code and heavily based all of my code on: https://repl.it/@cs344/65prodconspipelinec

pthread_cond_t full_1, full_2, full_3 = PTHREAD_COND_INITIALIZER;


/****************************
 ******* THREAD 4 START *****
 ***************************/
char * get_buff3() {                                 
   char * line = malloc(strlen(buff3[con_idx3]) + 1); //Allocate line to hold what we grab from the buffer
   
   strcpy(line, buff2[con_idx3]); 
   
   con_idx3++; //Iterate consumer index
   
   return line;
}

void * printBuff() {  //This is heavily based on Michael Slater's Pseudocode on Piazza: https://piazza.com/class/kjc3320l16c2f1?cid=465
   int last_line = 0; //Keep track of what has been printed
   char output[81] = {0}; //Buffer that will be printed to screen
   char * print_buffer = calloc(SIZE * NUM_LINES, sizeof(char)); //Load all the lines into one long line
   char * line = NULL; //Use this

   pthread_mutex_lock(&mutex_3);

   while(line3 == 0) { //If there's no items in buffer, wait
      pthread_cond_wait(&full_3, &mutex_3);
   }

   for(int i = 0; i < line3; i++) { //Load buffer 3 into the one big buffer
      line = get_buff3();
      strcat(print_buffer, line);
   }

   pthread_mutex_unlock(&mutex_3);

   int curr_line_count = (strlen(print_buffer) / 80); //Figure out how many complete lines we can print

   //Load 80 characters into the output buffer
   for(int t = 0; t < curr_line_count; t++) { 
      memset(output, 0, 81);                 
      strncpy(output, print_buffer + (t * 80), 80);
      printf("%s\n", output);
      fflush(stdout);
   }
   
   last_line = curr_line_count;
}
/**************************
 ******* THREAD 4 END *****
 *************************/


 /*************************
 ***** THREAD 3 START *****
 *************************/
char * get_buff2() { //Same get_buff function as above in thread 4
   char * line = malloc(strlen(buff2[con_idx2]) + 1);
   
   strcpy(line, buff2[con_idx2]);
   
   con_idx2++;
   
   return line;
}

void put_buff3(char * userInput) { //Copy the the line into the buffer and increment counter
   strcpy(buff3[line3], userInput); 
   
   line3++;
}

void convertPlus(char * userInput) { //I adapted a function from my smallsh assignment. It worked there but has weird behavior here
   char * buffer = strdup(userInput); //For example: ++rust++ turns into ^rust(weird character) if you print buffer 3
   
   for(int i = 0; i < strlen(userInput); i++) {
      if((buffer[i] == '+') && (i+1 < strlen(buffer)) && (buffer[i+1] == '+')) {
	 buffer[i] = '%';
	 buffer[i+1] = 'c';
      }
   }
   
   sprintf(userInput, buffer, '^');
}

void * changePlus() {
   char * line = NULL;
   char * temp = NULL;

   pthread_mutex_lock(&mutex_3);
   
   while(line2 == 0) { //If there's nothing in the buffer, wait
      pthread_cond_wait(&full_2, &mutex_2);
   }
   
   for(int i = 0; i < line2; i++) {
      line = get_buff2();
      convertPlus(line);
      put_buff3(line);
   }
   
   pthread_cond_signal(&full_3); //Signal buffer is full
   
   pthread_mutex_unlock(&mutex_3);
}
/**************************
 ******* THREAD 3 END *****
 *************************/



/****************************
 ******* THREAD 2 START *****
 ***************************/
char * get_buff1() { //Same as function in thread 4
   char * line = malloc(strlen(buff1[con_idx1]) + 1);
   
   strcpy(line, buff1[con_idx1]);
   
   con_idx1++;

   return line;
}

void put_buff2(char * userInput) { //Same as function in thread 3
   strcpy(buff2[line2], userInput);
   
   line2++;
}

void * separateLine() {
   char * line;
   while(stop != 1) { //While "STOP\n" has not been read
      
      pthread_mutex_lock(&mutex_2);
      
      for(int i = 0; i < line1; i++) {
	 line = get_buff1();
	 line[strcspn(line, "\n")] = ' '; //Replace \n with ' '
	 put_buff2(line);
      }
      
      pthread_cond_signal(&full_2);
      
      pthread_mutex_unlock(&mutex_2);
   }
}
/**************************
 ******* THREAD 2 END *****
 *************************/



/**************************
 ******THREAD 1 START *****
 *************************/
void put_buff1(char * userInput) { 
   strcpy(buff1[line1], userInput);
   
   line1++;
}

void* getUserInput() {
   size_t length = 0; //For getline
   char * input = NULL;

   //This is adapted from Michael Slater's post on Piazza: https://piazza.com/class/kjc3320l16c2f1?cid=458
   while(stop != 1) {
      getline(&input, &length, stdin);

      if(strncmp(input, "STOP\n", 5) == 0) { 
	 stop = 1; //Set flag if encounter "STOP\n"
      }else {
	 pthread_mutex_lock(&mutex_1);
	 
	 put_buff1(input);

	 pthread_cond_signal(&full_1);

	 pthread_mutex_unlock(&mutex_1);
      }
   }
}
/****************************
 ******* THREAD 1 END *******
****************************/


//Buffer printing functions
void print_buff1() {
   for(int i = 0; i < line1; i++) {
      printf("Buffer1[%i]: %s", i, buff1[i]);
      fflush(stdout);
   }
}

void print_buff2() {
   for(int i = 0; i < line2; i++) {
      printf("Buffer2[%i]: %s", i, buff2[i]);
      fflush(stdout);
   }
}

void print_buff3() {
   for(int i = 0; i < line3; i++) {
      printf("Buffer3[%i]: %s", i+1, buff3[i]);
      fflush(stdout);
   }
   printf("\n");
   fflush(stdout);
}

//I adapted this from the example thread code: https://repl.it/@cs344/65prodconspipelinec
int main() {
   pthread_t input_t, line_t, plus_t, output_t;

   pthread_create(&input_t, NULL, getUserInput, NULL);
   pthread_create(&line_t, NULL, separateLine, NULL);
   pthread_create(&plus_t, NULL, changePlus, NULL);
   pthread_create(&output_t, NULL, printBuff, NULL);
   
   
   pthread_join(input_t, NULL);
   pthread_join(line_t, NULL);
   pthread_join(plus_t, NULL);
   pthread_join(output_t, NULL);

   //print_buff1();
   //print_buff2();

   /*
   printf("*** BUFFER 3 ***\n");
   fflush(stdout);
   print_buff3();
   */

   return 0;
}
