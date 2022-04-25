#include "demo_functions.h"

//Returns 0 if sequence does not match, 1 otherwise
int checkSequence(int *inputSeq, int *actualSeq){
	int level = actualSeq[-1];
    
    int arr_length = inputSeq[-1];
    
    if(arr_length != level){
    	return 0;
    }
    
    for(int i = 0; i < arr_length; i++){
    	if(inputSeq[i] != actualSeq[i]){
    		return 0;
    	}
    }
    
    return 1;
}

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

void displayLightAndSoundForLedNumber(int ledNumber){
    int ledPin = decodePinFromLEDNumber(ledNumber);
    int frequency = decodeFrequencyFromLEDNumber(ledNumber);
    
    digitalWrite(ledPin, HIGH);
    softToneWrite(SPEAKER_PIN, frequency);
    delay(1000);
    digitalWrite(ledPin, LOW);
    softToneWrite(SPEAKER_PIN, 0);
    delay(500);
}

void displayLightAndSoundSequence(int *sequenceArray){
    int level = sequenceArray[-1];
    for(int sequenceIndex = 0; sequenceIndex < level; ++sequenceIndex){
        int ledNumber = sequenceArray[sequenceIndex];
        int ledPin = decodePinFromLEDNumber(ledNumber);
        int frequency = decodeFrequencyFromLEDNumber(ledNumber);
        
        digitalWrite(ledPin, HIGH);
        softToneWrite(SPEAKER_PIN, frequency);
        delay(1000);
        digitalWrite(ledPin, LOW);
        softToneWrite(SPEAKER_PIN, 0);
        delay(500);
    }  
}

void countDown(){
	for(int i = 3; i > 0; --i){
		printf("%d...\n", i);
		for(int j = 2; j <= 5; ++j)
			digitalWrite(j, HIGH);
		delay(400);
		for(int j = 2; j <= 5; ++j)
			digitalWrite(j, LOW);
		delay(400);
	}
}













