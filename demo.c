#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdint.h>
#include <sys/timerfd.h>
#include <time.h>			
#include <semaphore.h>
#include <wiringPi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <softTone.h>

#define NUM_THREADS 3
#define SPEAKER_PIN 6
#define MAX_LEVEL 10

unsigned long * GPEDS;
int *sequence;
int level = 3;

void *userInput(void *args);
void *generateSequence(void *args);
void *checkSequence(void *args);

void configurePins();

int main(void){

	wiringPiSetup();
	wiringPiSetupGpio();
	softToneCreate(SPEAKER_PIN);
	configurePins();	//Necessary initial setup functions
	
	sequence = malloc(MAX_LEVEL * sizeof(int));
	  
    pthread_t threads[NUM_THREADS];
    
    pthread_create(&(threads[0]), NULL, generateSequence, &(threads[0]));
    pthread_create(&(threads[1]), NULL, checkSequence, &(threads[1]));
    pthread_create(&(threads[2]), NULL, userInput, &(threads[2]));
    
    for(int i = 0; i < NUM_THREADS; ++i)
        pthread_join(threads[i], NULL);
        
    free(sequence);
}

void configurePins(){
	for(int i = 2; i <= 5; ++i) //Set all LEDs to Output
		pinMode(i, OUTPUT);
		
	for(int i = 16; i <= 20; ++i) { //Set all Buttons to Input & Enable Pull-Downs
		pinMode(i, INPUT);
		pullUpDnControl (i, PUD_DOWN);
	}
	
	//Initialize GPEDS button-press detection register
	int fd = open("/dev/mem", O_RDWR | O_SYNC);
    unsigned long * ptr = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x3F200000);
    GPEDS = ptr + 0x10;

    //GPEDS will initially have garbage value. This will remove it and initialize to 0    
    unsigned long temp = *GPEDS;
    *GPEDS = temp;
}

void * generateSequence(void *args){

    time_t t;
    
    //Initializes random number generator
    srand((unsigned) time(&t));
    
    for(int i = 0; i < level; ++i){
        
    }
    
    pthread_exit(0);
}


void *userInput(void *args){



    pthread_exit(0);
}



void *checkSequence(void *args){



    pthread_exit(0);
}















