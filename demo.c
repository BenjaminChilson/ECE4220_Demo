#include "demo_functions.h"

unsigned long * GPEDS;
int *sequence;
int level = 3;

sem_t sequenceReadWrite_control;

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

















