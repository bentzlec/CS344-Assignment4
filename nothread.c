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

int line1, line2, line3 = 0; //How many lines in each buffer

int con_idx1, con_idx2, con_idx3 = 0;

int stop = 0;

/*************************************************
 ******* ACTIONS PERFORMED BY THREAD 4 START *****
 ************************************************/
char * get_buff3() {
   char * line = malloc(strlen(buff3[con_idx3]) + 1);
   strcpy(line, buff2[con_idx3]);
   con_idx3++;
   
   return line;
}


void * printBuff() {
   int last_line = 0;
   char output[81] = {0};
   char * print_buffer = calloc(SIZE * NUM_LINES, sizeof(char));
   char * line = NULL;

   for(int i = 0; i < line3; i++) {
      line = get_buff3();
      strcat(print_buffer, line);
   }

   int curr_line_count = (strlen(print_buffer) / 80);

   for(int t = 0; t < curr_line_count; t++) {
      memset(output, 0, 81);
      strncpy(output, print_buffer + (t * 80), 80);
      printf("%s\n", output);
      fflush(stdout);
   }
   last_line = curr_line_count;
}
/*************************************************
 ******* ACTIONS PERFORMED BY THREAD 4 END *****
 ************************************************/


/*************************************************
 ******* ACTIONS PERFORMED BY THREAD 3 START *****
 ************************************************/
char * get_buff2() {
   char * line = malloc(strlen(buff2[con_idx2]) + 1);
   strcpy(line, buff2[con_idx2]);
   con_idx2++;
   
   return line;
}

void put_buff3(char * userInput) {
   strcpy(buff3[line3], userInput);
   line3++;
}

void convertPlus(char * userInput) {
   char * buffer = strdup(userInput);
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
   for(int i = 0; i < line2; i++) {
      line = get_buff2();
      convertPlus(line);
      put_buff3(line);
   }
}
/*************************************************
 ******* ACTIONS PERFORMED BY THREAD 3 END *****
 ************************************************/



/*************************************************
 ******* ACTIONS PERFORMED BY THREAD 2 START *****
 ************************************************/
char * get_buff1() {
   char * line = malloc(strlen(buff1[con_idx1]) + 1);
   strcpy(line, buff1[con_idx1]);
   con_idx1++;

   return line;
}

void put_buff2(char * userInput) {
   strcpy(buff2[line2], userInput);
   line2++;
}

void * separateLine() {
   char * line;
   for(int i = 0; i < line1; i++) {
      line = get_buff1();
      line[strcspn(line, "\n")] = ' ';
      put_buff2(line);
   }
}
/***********************************************
 ******* ACTIONS PERFORMED BY THREAD 2 END *****
 **********************************************/



/*************************************************
 * ***** ACTIONS PERFORMED BY THREAD 1 START *****
 ************************************************/
void put_buff1(char * userInput) {
   strcpy(buff1[line1], userInput);
   line1++;
}

void* getUserInput() {
   size_t length = 0;
   char * input = NULL;

   while(stop != 1) {
      getline(&input, &length, stdin);

      if(strncmp(input, "STOP\n", 5) == 0) {
	 stop = 1;
      }else {
	 put_buff1(input);
      }
   }
}
/***********************************************
 ******* ACTIONS PERFORMED BY THREAD 1 END *****
***********************************************/



void print_buff1() {
   for(int i = 0; i < line1; i++) {
      printf("Buffer1[%i]: %s", i, buff1[i]);
   }
}

void print_buff2() {
   for(int i = 0; i < line2; i++) {
      printf("Buffer2[%i]: %s", i, buff2[i]);
   }
   printf("\n");
}

void print_buff3() {
   for(int i = 0; i < line3; i++) {
      printf("Buffer3[%i]: %s", i, buff3[i]);
   }
   printf("\n");
}

int main() {
   getUserInput();
   separateLine();
   changePlus();

   print_buff3();
   printf("\n\n"); 
   printBuff();
   //printf("\n");
   //print_buff2();
   //print_buff3();

   return 0;
}
