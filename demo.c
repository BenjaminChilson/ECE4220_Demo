#include "demo_functions.h"

unsigned long * GPEDS;
sem_t sequenceReadWrite_control;
int state = 0;

int main(void){
    setupBoard();
    int fd = initializeGPEDS();
    int *sequence = initializeSequenceArray(3);
    
    //initialize semiphore utilized for thread control
    sem_init(&sequenceReadWrite_control, 0 , 1);
    
    pthread_t threads[NUM_THREADS];
    //spin up threads executing the needed methods of the game
    pthread_create(&(threads[0]), NULL, (void *)&generateSequence, (void *)sequence);
    pthread_create(&(threads[1]), NULL, (void *)&userInput, (void *)sequence);
    //collect all outstanding threads
    for(int i = 0; i < NUM_THREADS; ++i){
        pthread_join(threads[i], NULL);
    }

	//necessary memory cleanup at program termination
    free(--sequence);
    close(fd);
}

void *userInput(void *args){

    int *actualSeq = (int *)args;
	int *inputSeq = initializeSequenceArray(0);
	
	//delay necessary so that userInput is not the first thread to activate
	usleep(10000);
    while(1){
        sem_wait(&sequenceReadWrite_control);
        if(state == EXIT){
        	sem_post(&sequenceReadWrite_control);
        	break;
        }
        if(state == RUNNING){
		    int i = 0;
		    int ledNumber = -1;
		    while(*GPEDS != BTN5_PRESSED){
				//reset GPEDS Register
				*GPEDS = *GPEDS;
			
				ledNumber = checkGPEDS(*GPEDS);
			
				if(ledNumber != -1){
					displayLightAndSoundForLedNumber(ledNumber);
					if(i < MAX_LEVEL){
		                inputSeq[i] = ledNumber;
		            }
					i++;
				}
		    }
		    *GPEDS = BTN5_PRESSED;
		    inputSeq[-1] = i;
		    
		    
		    if(checkSequence(inputSeq, actualSeq)){
				printf("Level %d completed!\n", actualSeq[-1] - 2);
			} else {
				state = FAIL;
			}
		}
        
        sem_post(&sequenceReadWrite_control);
        usleep(10000);
    }
    free(--inputSeq);
    pthread_exit(0);
}

void *generateSequence(void *args){
    int *sequence = (int *)args;
    int level = sequence[-1];
    time_t t;
        
    //Initializes random number generator
    srand((unsigned) time(&t));
	
    while(1){
        sem_wait(&sequenceReadWrite_control);
        
        if(state == EXIT){
        	sem_post(&sequenceReadWrite_control);
        	break;
        }
        
        switch(state){
        	case START:
		    	displayMenu(state);
				while(*GPEDS != BTN1_PRESSED){
					*GPEDS = *GPEDS;
				}
				level = 3;
				state = RUNNING;
        		break;
        	case FAIL:
        	case WIN:
        		displayMenu(state);
        		while(*GPEDS != BTN1_PRESSED && *GPEDS != BTN2_PRESSED){
        			*GPEDS = *GPEDS;
        		}
        		if(*GPEDS == BTN1_PRESSED){
        			state = START;
				}
				else{
					state = EXIT;
					printf("Goodbye\n");
				}
        		break;
        }
        *GPEDS = *GPEDS;
           
        if(state == RUNNING){    
		    sequence[-1] = level;
		    if(level <= 3){
		        for(int i = 0; i < level; ++i){
		            sequence[i] = rand() % 4;
		        }
		    }
		    else{
		        sequence[level - 1] = rand() % 4;
		    }

			countDown(level - 2);
			displayLightAndSoundSequence(sequence);
			level++;
        }
        sem_post(&sequenceReadWrite_control);
	    usleep(10000);
    }

    pthread_exit(0);
}

int initializeGPEDS(){
    //Initialize GPEDS button-press detection register
	int fd = open("/dev/mem", O_RDWR | O_SYNC);
    unsigned long * ptr = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x3F200000);
    GPEDS = ptr + 0x10;
    
    *GPEDS = *GPEDS;
    
    return fd;
}










