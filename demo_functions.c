#include "demo_functions.h"

//Returns 0 if sequence does not match, 1 otherwise
int checkSequence(int *inputSeq, int *actualSeq){
	int level = actualSeq[-1];
    
    int arr_length = inputSeq[0];
	inputSeq++;
    
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
    printf("here3_1\n");
    int level = sequenceArray[-1];
    printf("here3_2\n");
    for(int sequenceIndex = 0; sequenceIndex < level; ++sequenceIndex){
        int ledNumber = sequenceArray[sequenceIndex];
        int ledPin = decodePinFromLEDNumber(ledNumber);
        int frequency = decodeFrequencyFromLEDNumber(ledNumber);
        printf("here3_3\n");
        
        digitalWrite(ledPin, HIGH);
        softToneWrite(SPEAKER_PIN, frequency);
        delay(1000);
        digitalWrite(ledPin, LOW);
        softToneWrite(SPEAKER_PIN, 0);
        delay(500);
    }  
}

void configurePins(unsigned long *GPEDS){
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
    resetGPEDS(GPEDS);
}


void resetGPEDS(unsigned long *GPEDS){
	unsigned long temp = *GPEDS;
	*GPEDS = temp;
}







