#include <iostream>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define NUM_THREADS 2 // Two threads in this program

pthread_t threads[NUM_THREADS];

// First, we list our two sempahores
sem_t sem1; // Create semaphore sem1
sem_t sem2; // Create semaphore sem2

// We'll store the heights for each person here
float fredHeight = 1; // Fred (Person A) starts on the low end, one foot off the ground
float wilmaHeight = 7; // Wilma (Persona B) starts on the high end, seven feet off the ground

void *fredSee(void*) { // Fred's Behavior

    while(true) {
        cout << "Fred Height: " << fredHeight << endl;
        sem_wait(&sem1); // Grab the semaphore for sem1 when allowed, and lock it
            if(fredHeight < 7)
                fredHeight += 1;
            else if(fredHeight > 1)
                fredHeight -= 1.5;
        sem_post(&sem2);
        sleep(1);


    }

    pthread_exit(nullptr);

}
void *wilmaSaw(void*) { // Wilma's Behavior

    while(true) {
        cout << "Wilma Height: " << wilmaHeight << endl;
        sem_wait(&sem2); // Grab the semaphore for sem1 when allowed, and lock it

        if(wilmaHeight > 1)
            wilmaHeight -= 1;
        else if(wilmaHeight < 7)
            wilmaHeight += 1.5;
        sem_post(&sem1);
        sleep(1);


    }

    pthread_exit(nullptr);
}

void checkForThreadErrors(int tid) {
    if (tid) {
        cout << "Error: Problem creating thread," << tid << endl;
        exit(-1);
    }
}

int main(int argc, char** argv) {

    sem_init(&sem1, 1, 1); // Initialize sem1 semaphore, shared between threads, and initialize value to 1
    sem_init(&sem2, 1, 0); // Initialize sem2 semaphore, shared between threads, and initialize value to 1

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