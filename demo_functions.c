#include "demo_functions.h"

void *userInput(void *args){
}

void *generateSequence(void *args){
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

void *checkSequence(void *args){
}

int decodePinFromLEDNumber(int ledNumber){
}

int decodeFrequencyFromLEDNumber(int ledNumber){
}

void displayLightAndSound(int sequenceIndex){
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
