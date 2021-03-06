#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdint.h>
#include <sys/timerfd.h>
#include <time.h>			
#include <semaphore.h>
#include <wiringPi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <softTone.h>
#include <limits.h>

#define NUM_THREADS 2
#define RED_LED_PIN 2
#define YELLOW_LED_PIN 3
#define GREEN_LED_PIN 4
#define BLUE_LED_PIN 5

#define RED_LED_FREQUENCY 200
#define YELLOW_LED_FREQUENCY 400
#define GREEN_LED_FREQUENCY 600
#define BLUE_LED_FREQUENCY 800

#define BTN1_PRESSED 0x10000
#define BTN2_PRESSED 0x20000
#define BTN3_PRESSED 0x40000
#define BTN4_PRESSED 0x80000
#define BTN5_PRESSED 0x100000

#define START 0
#define FAIL 1
#define WIN 2
#define EXIT 3
#define RUNNING 4

#define SPEAKER_PIN 6
#define MAX_LEVEL 10

//Thread Functions
void *userInput(void *args);
void *generateSequence(void *args);
//Helper Functions
int checkSequence(int *input, int *actualSeq);
int decodePinFromLEDNumber(int ledNumber);
int decodeFrequencyFromLEDNumber(int ledNumber);
void displayLightAndSoundForLedNumber(int ledNumber);
void displayLightAndSoundSequence(int *sequenceArray);
void configurePins();
void setupBoard();
int initializeGPEDS();
int *initializeSequenceArray();

int checkGPEDS(int GPEDS);
void countDown(int level);
void displayStartupMenu();
void displayFailureMenu();
void displayVictoryMenu();
void displayOptionsMenu();
void displayMenu(int state);







