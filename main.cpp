#include <iostream>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define NUM_THREADS 2 // Two threads in this program

pthread_t threads[NUM_THREADS];

// First, we list our two sempahores
sem_t increaseHeight; // Create semaphore increaseHeight
sem_t decreaseHeight; // Create semaphore decreaseHeight

// We'll store the heights for each person here
float fredHeight = 1; // Fred (Person A) starts on the low end, one foot off the ground
float wilmaHeight = 7; // Wilma (Persona B) starts on the high end, seven feet off the ground

void *fredSee(void*) { // Fred's Behavior

    // Go up and down 10 times
    for (int i = 0; i < 10; i++) {
        // Go up
        sem_wait(&increaseHeight); // Grab the semaphore for increaseHeight when allowed, and lock it
        while(fredHeight < 7) { // Maximum height is 7 feet
            fredHeight += 1;
            cout << "Fred Height: " << fredHeight << endl;
            sleep(1);
        }

        sem_post(&increaseHeight); // Release the semaphore for increase height
        // Go down
        sem_wait(&decreaseHeight);
        while(fredHeight > 1) { // Check to see if fred is within a foot off the ground. If not, decrease height
            fredHeight -= 1.5;
            cout << "Fred Height: " << fredHeight << endl;
            sleep(1);
        }

        sem_post(&decreaseHeight); // Release the semaphore for decrease height
    }

    cout << "first thread" << endl;
    pthread_exit(nullptr);

}
void *wilmaSaw(void*) { // Wilma's Behavior

    // Go up and down 10 times
    for (int i = 0; i < 10; i++) {
        // Go down (when you are allowed to
        sem_wait(&decreaseHeight);
        while(wilmaHeight > 1) {
            wilmaHeight -= 1;
            cout << "Wilma Height: " << wilmaHeight << endl;
            sleep(1);
        }

        sem_post(&decreaseHeight); // Release the semaphore for decrease height
        // Go up (when you are allowed to go up because you are
        sem_wait(&increaseHeight); // Grab the semaphore for increaseHeight when allowed, and lock it
        while(wilmaHeight < 7) { // Maximum height is 7 feet
            wilmaHeight += 1.5;
            cout << "Wilma Height: " << wilmaHeight << endl;
            sleep(1);
        }

        sem_post(&increaseHeight); // Release the semaphore for increase height
    }

    cout << "second thread" << endl;
    pthread_exit(nullptr);
}

void checkForThreadErrors(int tid) {
    if (tid) {
        cout << "Error: Problem creating thread," << tid << endl;
        exit(-1);
    }
}

int main(int argc, char** argv) {

    sem_init(&increaseHeight, 1, 1); // Initialize increaseHeight semaphore, shared between threads, and initialize value to 1
    sem_init(&decreaseHeight, 1, 1); // Initialize decreaseHeight semaphore, shared between threads, and initialize value to 1

    int tid;

    // Create Threads

    // Thread A (Fred)
    tid = pthread_create(&threads[0], nullptr, fredSee, nullptr); // Thread for Fred
    checkForThreadErrors(tid); // Check for threading errors

    // Thread B (Wilma)
    tid = pthread_create(&threads[1], nullptr, wilmaSaw, nullptr); // Thread for Wilma
    checkForThreadErrors(tid); // Check for threading errors

    // Join all threads: Making sure that all have completed before continuing onwards
    for (unsigned long thread : threads) {
        pthread_join(thread, nullptr); // Join threads
    }




    return 0;



}