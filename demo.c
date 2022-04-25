#include "demo_functions.h"

unsigned long * GPEDS;
sem_t sequenceReadWrite_control;

int main(void){

	wiringPiSetup();
	wiringPiSetupGpio();
	softToneCreate(SPEAKER_PIN);
	configurePins(GPEDS);	//Necessary initial setup functions
	
	//Define starting sequence whose initial length is stored in index -1,
	//but has space for an end-game sequence of length MAX_LEVEL
	int *sequence = malloc(MAX_LEVEL + 1 * sizeof(int));
	sequence[0] = 3;
	sequence++;
	
    sem_init(&sequenceReadWrite_control, 0 , 1);
	  
    pthread_t threads[NUM_THREADS];
    
    pthread_create(&(threads[0]), (void *)sequence, generateSequence, &(threads[0]));
    pthread_create(&(threads[1]), (void *)sequence, userInput, &(threads[1]));
    
    for(int i = 0; i < NUM_THREADS; ++i)
        pthread_join(threads[i], NULL);
        
    free(sequence);
}


void *userInput(void *args){
    int *actualSeq = (int *)args;
	int *inputArr = malloc((MAX_LEVEL + 1) * sizeof(int));
	inputArr++;
    while(1){
        sem_wait(&sequenceReadWrite_control);
        int i = 0;
        int ledNumber = -1;
        while(*GPEDS != BTN5_PRESSED){
			switch(*GPEDS){
				case BTN1_PRESSED:
					ledNumber = 0;
					break;
				
				case BTN2_PRESSED:
					ledNumber = 1;
					break;
				
				case BTN3_PRESSED:
					ledNumber = 2;
					break;
				
				case BTN4_PRESSED:
					ledNumber = 3;
					break;
				
				default:
					ledNumber = -1;
					printf("Invalid input, please try again.\n");
					break;
				
			}
			if(ledNumber != -1){
				displayLightAndSoundForLedNumber(ledNumber);
				if(i < MAX_LEVEL){
                    inputArr[i] = ledNumber;
                }
				i++;
			}
			resetGPEDS(GPEDS);
        }
        inputArr[-1] = i;
        checkSequence(inputArr, actualSeq);
        sem_post(&sequenceReadWrite_control);
    }
    free(inputArr);
    pthread_exit(0);
}

void *generateSequence(void *args){
    int *sequence = (int *)args + 1;
    int level = 3;
    while(1){
        sem_wait(&sequenceReadWrite_control);
        sequence[-1] = level;
        time_t t;
        
        //Initializes random number generator
        srand((unsigned) time(&t));

        for(int i = 0; i < level; ++i){
            sequence[i] = rand() % 4;
        }
        displayLightAndSoundSequence(sequence);
        level++;
        sem_post(&sequenceReadWrite_control);
    }
    
    pthread_exit(0);
}

















