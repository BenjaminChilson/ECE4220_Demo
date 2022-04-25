#include "demo_functions.h"

unsigned long * GPEDS;
sem_t sequenceReadWrite_control;

int main(void){
	wiringPiSetup();
	wiringPiSetupGpio();
	softToneCreate(SPEAKER_PIN);
	configurePins();	//Necessary initial setup functions
	
	//Initialize GPEDS button-press detection register
	int fd = open("/dev/mem", O_RDWR | O_SYNC);
    unsigned long * ptr = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x3F200000);
    GPEDS = ptr + 0x10;

	//Define starting sequence whose initial length is stored in index -1,
	//but has space for an end-game sequence of length MAX_LEVEL
	int *sequence = malloc(MAX_LEVEL + 1 * sizeof(int));
    if(!sequence){
        printf("malloc failed!!!!\n");
    }
	sequence[0] = 3;
	sequence++;
	
    sem_init(&sequenceReadWrite_control, 0 , 1);
	  
    pthread_t threads[NUM_THREADS];
    
    pthread_create(&(threads[0]), NULL, (void *)&generateSequence, (void *)sequence);
    pthread_create(&(threads[1]), NULL, (void *)&userInput, (void *)sequence);
    
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);


    free(sequence);
    close(fd);
    pthread_exit(NULL);
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
        
			ledNumber = checkGPEDS();
			
			//reset GPEDS Register
			unsigned long temp = *GPEDS;
			*GPEDS = temp;
			
			if(ledNumber != -1){
				displayLightAndSoundForLedNumber(ledNumber);
				if(i < MAX_LEVEL){
                    inputArr[i] = ledNumber;
                }
				i++;
			}
        }
        *GPEDS = BTN5_PRESSED;
        
        inputArr[-1] = i;
        
        
        if(checkSequence(inputArr, actualSeq)){
			printf("Level %d completed!\n", actualSeq[-1] - 2);
		} else {
			printf("Failure at Level %d!\n", actualSeq[-1] - 2);
		}
        
        sem_post(&sequenceReadWrite_control);
        usleep(10000);
    }
    free(inputArr);
    pthread_exit(0);
}

void *generateSequence(void *args){
    int *sequence = (int *)args;
    int level = 3;
    while(level <= MAX_LEVEL){
        sem_wait(&sequenceReadWrite_control);
        sequence[-1] = level;
        time_t t;
        
        //Initializes random number generator
        srand((unsigned) time(&t));
        for(int i = 0; i < level; ++i){
            sequence[i] = rand() % 4;
        }
        
        countDown();
        displayLightAndSoundSequence(sequence);
        level++;
        sem_post(&sequenceReadWrite_control);
        usleep(10000);
    }
    printf("Fantastic! You have completed the tutorial.");
    printf("Please enter your mom's credit card information to continue playing.\n");
    
    pthread_exit(0);
}

void configurePins(){
	for(int i = 2; i <= 5; ++i) //Set all LEDs to Output
		pinMode(i, OUTPUT);
		
	for(int i = 16; i <= 20; ++i) { //Set all Buttons to Input & Enable Pull-Downs
		pinMode(i, INPUT);
		pullUpDnControl (i, PUD_DOWN);
	}
}

int checkGPEDS(){
	switch(*GPEDS){
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
		
		default:
			printf("Invalid input, please try again.\n");
			return -1;
			break;
	}
}











