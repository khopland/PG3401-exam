#include "debugger.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFERSIZE 4096

void *TaskA(void *pvData);
void *TaskB(void *pvData);
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct _THREADOPTIONS {
    bool dataFlag;
    bool doneFlag;
    char Buffer[BUFFERSIZE];
} THREADOPTIONS;

int main(void) {
    pthread_t pThreadA, pThreadB;
    THREADOPTIONS *params = (THREADOPTIONS *)malloc(sizeof(THREADOPTIONS));

    memset(params, 0, sizeof(THREADOPTIONS));

    if (pthread_create(&pThreadA, NULL, TaskA, params) != 0) {
        error("failed to create pThreadA");
        return 1;
    }

    if (pthread_create(&pThreadB, NULL, TaskB, params) != 0) {
        error("failed to create pThreadB");
        return 1;
    }
    pthread_join(pThreadA, NULL); // Wait for thread A to finish
    pthread_join(pThreadB, NULL); // Wait for thread B to finish
    free(params);
    params = NULL;

    return 0;
}
// task for thread A
void *TaskA(void *pvData) {
    char *fileName = "PG3401-Hjemmeeksamen-14dager-H22.pdf";
    THREADOPTIONS *options = (THREADOPTIONS *)pvData;
    char buff[BUFFERSIZE] = {0};
    FILE *f = NULL;

    f = fopen(fileName, "r");

    do {
        usleep(1); // needs this for no dead lock
        if (options->dataFlag == false) {
            if (feof(f)) { // check if its end of file, if so set done flag and break
                pthread_mutex_lock(&lock);
                options->doneFlag = true;
                pthread_mutex_unlock(&lock);
                break;
            }
            memset(buff, 0, BUFFERSIZE);
            if (fgets(buff, BUFFERSIZE, f) != NULL) { // reads from pdf file and sets it to the sheared buffer
                pthread_mutex_lock(&lock);
                memset(options->Buffer, 0, BUFFERSIZE);
                strcpy(options->Buffer, buff);
                options->dataFlag = true; // sets the data flag to true
                pthread_mutex_unlock(&lock);
            }
        }
    } while (true);

    fclose(f); // close file
    return NULL;
}

void *TaskB(void *pvData) {
    THREADOPTIONS *options = (THREADOPTIONS *)pvData;
    long array[256] = {0}; // local array to stor the amount of bytes
    int i = 0;
    do {
        if (options->dataFlag == true) {
            pthread_mutex_lock(&lock);
            for (i = 0; i < BUFFERSIZE; i++) { // read the bytes and add it to the array
                array[(long)options->Buffer[i]] += 1;
            }

            memset(options->Buffer, 0, BUFFERSIZE);
            options->dataFlag = false; // sets the data flag to false
            pthread_mutex_unlock(&lock);
        }
        if (options->doneFlag == true) { // check if task a is done
            break;
        }
    } while (true);
    for (i = 0; i < 256; i++) { // print the data
        printf("%02X has %li\n", i, array[i]);
    }
    return NULL;
}