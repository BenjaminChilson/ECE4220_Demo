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
    
    pthread_create(&(threads[0]), sequence, generateSequence, &(threads[0]));
    pthread_create(&(threads[1]), NULL, userInput, &(threads[1]));
    
    for(int i = 0; i < NUM_THREADS; ++i)
        pthread_join(threads[i], NULL);
        
    free(sequence);
}

void *userInput(void *args){
	int inputArr[MAX_LEVEL +1];
	inputArr++;
    while(1){
        sem_wait(&sequenceReadWrite_control);
        int i = 0;
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
				inputArr[i] = ledNumber;
				i++;
			}
			resetGPEDS(GPEDS);
        }
        
        sem_post(&sequenceReadWrite_control);
    }
	inputArr[-1] = i;

    pthread_exit(0);
}

















