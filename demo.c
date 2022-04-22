#include "demo_functions.h"

unsigned long * GPEDS;
int *sequence;
int level = 3;

sem_t sequenceReadWrite_control;

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

void *userInput(void *args){
    while(1){
        sem_wait(&sequenceReadWrite_control);
        
        sem_post(&sequenceReadWrite_control);
    }


    pthread_exit(0);
}



void *checkSequence(void *args){
    while(1){
        sem_wait(&sequenceReadWrite_control);
        int sequenceIndex = 0;
        int falseInput = 0;
        while(*GPEDS != BTN5_PRESSED){
            switch(*GPEDS){
                case BTN1_PRESSED:
                case BTN2_PRESSED:
                case BTN3_PRESSED:
                case BTN4_PRESSED:
                    if(sequenceIndex < MAX_LEVEL){
                        falseInput = 1;
                    }
                    else{
                        falseInput = compareInputToSequence(sequenceIndex);
                    }
                    sequenceIndex++;
                    *GPEDS = *GPEDS;
                    break;
            }
        }
        if(sequenceIndex != level && sequence[sequenceIndex] != -1 && falseInput != 0){

        }
        sem_post(&sequenceReadWrite_control);
    }
    pthread_exit(0);
}















