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
#define RED_LED_PIN 2
#define YELLOW_LED_PIN 3
#define GREEN_LED_PIN 4
#define BLUE_LED_PIN 5

#define RED_LED_FREQUENCY 200
#define YELLOW_LED_FREQUENCY 400
#define GREEN_LED_FREQUENCY 600
#define BLUE_LED_FREQUENCY 800

#define SPEAKER_PIN 6
#define MAX_LEVEL 10

unsigned long * GPEDS;
int *sequence;
int level = 3;

sem_t sequenceReadWrite_control;

void *userInput(void *args);
void *generateSequence(void *args);
void *checkSequence(void *args);



int decodePinFromLEDNumber(int ledNumber){
    switch(ledNumber){
        case 0:
            return RED_LED_PIN;
        case 1:
            return YELLOW_LED_PIN;
        case 2:
            return GREEN_LED_PIN;
        case 3:
            return BLUE_LED_PIN;
        default:
            //led number passed is not an accepted value
            return -1;
    }
}

int decodeFrequencyFromLEDNumber(int ledNumber){
    switch(ledNumber){
        case 0:
            return RED_LED_FREQUENCY;
        case 1:
            return YELLOW_LED_FREQUENCY;
        case 2:
            return GREEN_LED_FREQUENCY;
        case 3:
            return BLUE_LED_FREQUENCY;
        default:
            //led number passed is not an accepted value
            return -1;
    }
}

void displayLightAndSound(int sequenceIndex){
    int ledNumber = sequence[sequenceIndex];
    int ledPin = decodePinFromLEDNumber(ledNumber);
    int frequency = decodeFrequencyFromLEDNumber(ledNumber);
    
    digitalWrite(ledPin, HIGH);
    softToneWrite(SPEAKER_PIN, frequency);
    delay(1000);
    digitalWrite(ledPin, LOW);
    softToneWrite(SPEAKER_PIN, 0);
    delay(500);
}
void configurePins();

int main(void){

	wiringPiSetup();
	wiringPiSetupGpio();
	softToneCreate(SPEAKER_PIN);
	configurePins();	//Necessary initial setup functions
	
	sequence = malloc(MAX_LEVEL * sizeof(int));
    sem_init(&sequenceReadWrite_control, 0 , 2);
	  
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
    while(1){
        sem_wait(&sequenceReadWrite_control);
        sem_wait(&sequenceReadWrite_control);
        time_t t;
        
        //Initializes random number generator
        srand((unsigned) time(&t));

        for(int i = 0; i < level; ++i){
            sequence[i] = rand() % 4;
        }
        for(int sequenceIndex = 0; sequenceIndex < level; ++sequenceIndex){
            displayLightAndSound(sequenceIndex);
        }
        sem_post(&sequenceReadWrite_control);
        sem_post(&sequenceReadWrite_control);
    }
    
    pthread_exit(0);
}


void *userInput(void *args){
    while(1){
        sem_wait(&sequenceReadWrite_control);
        int sequenceIndex = 0;
        int sequenceValid = 0;
        while(*GPEDS != BTN5_PRESSED){
            sequenceValid = compareInputToSequence(sequenceIndex);

        }
        sem_post(&sequenceReadWrite_control);
    }


    pthread_exit(0);
}



void *checkSequence(void *args){
    while(1){
        sem_wait(&sequenceReadWrite_control);
        
        sem_post(&sequenceReadWrite_control);
    }
    pthread_exit(0);
}















