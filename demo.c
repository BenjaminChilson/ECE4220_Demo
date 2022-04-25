#include "demo_functions.h"

unsigned long * GPEDS;
sem_t sequenceReadWrite_control;

int main(void){
    printf("here\n");
	wiringPiSetup();
	wiringPiSetupGpio();
	softToneCreate(SPEAKER_PIN);
	configurePins(GPEDS);	//Necessary initial setup functions
	printf("here1\n");
	//Define starting sequence whose initial length is stored in index -1,
	//but has space for an end-game sequence of length MAX_LEVEL
	int *sequence = malloc(MAX_LEVEL + 1 * sizeof(int));
    if(!sequence){
        printf("malloc failed!!!!\n");
    }
	sequence[0] = 3;
	sequence++;
	printf("here2\n");
    sem_init(&sequenceReadWrite_control, 0 , 1);
	  
    pthread_t threads[NUM_THREADS];
    pthread_attr_t a;
    size_t stacksize;
    pthread_attr_setstacksize(&a, 10000000);
    pthread_attr_getstacksize(&a, &stacksize);
    printf("Current stack size = %d\n", stacksize);

    printf("here4\n");
    *GPEDS = *GPEDS;
    pthread_create(&(threads[0]), NULL, (void *)&generateSequence, (void *)sequence);
    int b = pthread_create(&(threads[1]), NULL, (void *)&userInput, (void *)sequence);
    printf("returns: %d\n", b);
    printf("here5\n");
    
    printf("here6\n");
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    // for(int i = 0; i < NUM_THREADS; ++i)
    //     pthread_join(threads[i], NULL);
        
    free(sequence);
}


void *userInput(void *args){
    printf("here1_1\n");
    int *actualSeq = (int *)args;
    printf("here1_2\n");
	int *inputArr = malloc((MAX_LEVEL + 1) * sizeof(int));
	inputArr++;
    printf("here1_3\n");
    while(1){
        sem_wait(&sequenceReadWrite_control);
        int i = 0;
        int ledNumber = -1;
        //printf("here1_4\n");
        while(*GPEDS != BTN5_PRESSED){
			switch(*GPEDS){
				case 0:
                    ledNumber = -1;
                    break;
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
            //printf("here1_5\n");
			if(ledNumber != -1){
				displayLightAndSoundForLedNumber(ledNumber);
				if(i < MAX_LEVEL){
                    inputArr[i] = ledNumber;
                }
				i++;
			}
            //printf("here1_6\n");
			*GPEDS = *GPEDS;
            //printf("here1_7\n");
        }
        *GPEDS = *GPEDS;

        inputArr[-1] = i;
        checkSequence(inputArr, actualSeq);
        sem_post(&sequenceReadWrite_control);
    }
    free(inputArr);
    pthread_exit(0);
}

void *generateSequence(void *args){
    printf("here2_1\n");
    int *sequence = (int *)args;
    int level = 3;
    while(1){
        sem_wait(&sequenceReadWrite_control);
        printf("here2_2\n");
        sequence[-1] = level;
        time_t t;
        
        //Initializes random number generator
        srand((unsigned) time(&t));
        printf("here2_3\n");
        for(int i = 0; i < level; ++i){
            sequence[i] = rand() % 4;
        }
        printf("here2_4\n");
        displayLightAndSoundSequence(sequence);
        level++;
        printf("here2_5\n");
        sem_post(&sequenceReadWrite_control);
        //printf("here2_6\n");
    }
    
    pthread_exit(0);
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
    *GPEDS = *GPEDS;
}

void resetGPEDS(){
	unsigned long temp = *GPEDS;
	*GPEDS = temp;
}