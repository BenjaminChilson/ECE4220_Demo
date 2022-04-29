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

void countDown(int level){
	printf("Level %d starting in\n", level);

	for(int i = 3; i > 0; --i){
		printf("%d...\n", i);
		for(int j = 2; j <= 5; ++j)
			digitalWrite(j, HIGH);
		delay(500);
		for(int j = 2; j <= 5; ++j)
			digitalWrite(j, LOW);
		delay(500);
	}
}

void setupBoard(){
    wiringPiSetup();
	wiringPiSetupGpio();
	softToneCreate(SPEAKER_PIN);
	configurePins();	//Necessary initial setup functions
}

void configurePins(){
	for(int i = 2; i <= 5; ++i) //Set all LEDs to Output
		pinMode(i, OUTPUT);
		
	for(int i = 16; i <= 20; ++i) { //Set all Buttons to Input & Enable Pull-Downs
		pinMode(i, INPUT);
		pullUpDnControl (i, PUD_DOWN);
	}
}

int *initializeSequenceArray(int sequenceCount){
    //Define starting sequence whose initial length is stored in index -1,
	//but has space for an end-game sequence of length MAX_LEVEL
	int *sequence = malloc(MAX_LEVEL + 1 * sizeof(int));
	sequence[0] = sequenceCount;
	sequence++;
    return sequence;
}

int checkGPEDS(int GPEDS){
	switch(GPEDS){
		case 0:
			return -1;
			break;
		case BTN1_PRESSED:
			return 0;
			break;
		
		case BTN2_PRESSED:
			return 1;
			break;
		
		case BTN3_PRESSED:
			return 2;
			break;
		
		case BTN4_PRESSED:
			return 3;
			break;
			
		case BTN5_PRESSED:
			return -1;
			break;
		
		default:
			printf("Invalid input, please try again.\n");
			return -1;
			break;
	}
}

void displayStartupMenu(){
	printf("*********************************\n"); //33 stars
	
	printf("*\tSimon Says Game\t\t*\n");
	printf("*\tBy Brennan Williams &\t*\n");
	printf("*\tBenjamin Chilson\t*\n");
	
	printf("**** Press Button 1 to Begin ****\n"); //33 stars
}

void displayFailureMenu(){
	int lineWidth = 24;
	for(int starCount = 2; starCount < lineWidth; starCount += 4){
		for(int i = 0; i < starCount / 2; i++){
			printf("*");
		}
		for(int i = 0; i < lineWidth - starCount; i++){
			printf(" ");
		}
		for(int i = 0; i < starCount / 2; i++){
			printf("*");
		}
		printf("\n");
	}
	
	for(int i = 0; i < (lineWidth - 12) / 2; i++){
		printf("*");
	}
	
	printf(" You Failed ");
	
	for(int i = 0; i < (lineWidth - 12) / 2; i++){
		printf("*");
	}
	printf("\n");
	
	for(int starCount = lineWidth - 2; starCount > 0; starCount -= 4){
		for(int i = 0; i < starCount / 2; i++){
			printf("*");
		}
		for(int i = 0; i < lineWidth - starCount; i++){
			printf(" ");
		}
		for(int i = 0; i < starCount / 2; i++){
			printf("*");
		}
		printf("\n");
	}
}

void displayVictoryMenu(){
	int lineWidth = 24;
	
	for(int starCount = lineWidth - 2; starCount > 0; starCount -= 4){
		for(int i = 0; i < starCount / 2; i++){
			printf(" ");
		}
		for(int i = 0; i < lineWidth - starCount; i++){
			printf("*");
		}
		for(int i = 0; i < starCount / 2; i++){
			printf(" ");
		}
		printf("\n");
	}
	for(int i = 0; i < (lineWidth - 10) / 2; i++){
		printf("*");
	}
	
	printf(" You Win! ");
	
	for(int i = 0; i < (lineWidth - 10) / 2; i++){
		printf("*");
	}
	printf("\n");
	for(int starCount = 2; starCount < lineWidth; starCount += 4){
		for(int i = 0; i < starCount / 2; i++){
			printf(" ");
		}
		for(int i = 0; i < lineWidth - starCount; i++){
			printf("*");
		}
		for(int i = 0; i < starCount / 2; i++){
			printf(" ");
		}
		printf("\n");
	}
}

void displayOptionsMenu(){
	printf("*********************************\n"); //33 stars

	printf("*   Options:\t\t\t*\n");
	printf("*   Button 1 to return to menu\t*\n");
	printf("*   Button 2 to exit game\t*\n");

	printf("*********************************\n"); //33 stars
}

void displayMenu(int state){

	switch(state){
		case 0:
			displayStartupMenu();
			break;
		case 1:
			displayFailureMenu();
			displayOptionsMenu();
			break;
		case 2:
			displayVictoryMenu();
			displayOptionsMenu();
			break;
		case 3:
			break;
		default:
			printf("Error, invalid game state\n");
			break;
	}
}




















