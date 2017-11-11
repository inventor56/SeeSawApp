#include <iostream>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define NUM_THREADS 2 // Two threads in this program
#define MIN_HEIGHT 1 // Minimum height a person can be at
#define MAX_HEIGHT 7 // Maximum height a person can be at

pthread_t threads[NUM_THREADS];

// First, we list our two sempahores
sem_t sem1; // Create semaphore sem1
sem_t sem2; // Create semaphore sem2

int upDown = 0;

// Store velocity
float velocity;

// We'll store the heights for each person here
float fredHeight = 1; // Fred (Person A) starts on the low end, one foot off the ground
float wilmaHeight = 7; // Wilma (Person B) starts on the high end, seven feet off the ground

void *fredSee(void*) { // Fred's Behavior

    while(upDown < 9) {
        sem_wait(&sem1);
        velocity = 1;
        while(fredHeight < MAX_HEIGHT && wilmaHeight > MIN_HEIGHT) {
            cout << "Fred Height: " << fredHeight << endl;
            cout << "Wilma Height: " << wilmaHeight << endl << "\n";
            fredHeight += velocity;
            wilmaHeight -= velocity;
            sleep(1);

        }
        sem_post(&sem2);
    }

    pthread_exit(nullptr);

}
void *wilmaSaw(void*) { // Wilma's Behavior

    while(upDown < 10) {
        sem_wait(&sem2);
        velocity = 1.5;
        while(wilmaHeight < MAX_HEIGHT && fredHeight > MIN_HEIGHT) {
            cout << "Fred Height: " << fredHeight << endl;
            cout << "Wilma Height: " << wilmaHeight << endl << "\n";
            wilmaHeight += velocity;
            fredHeight -= velocity;
            sleep(1);

        }
        upDown++;
        cout << "Iteration: " << upDown << " Complete\n\n";
        sem_post(&sem1);
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
    sem_init(&sem2, 1, 0); // Initialize sem2 semaphore, shared between threads, and initialize value to 0

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

    // Simulation has ended: Final see-saw state printed below
    cout << "Finished with Fred Height: " << fredHeight << " Wilma Height: " << wilmaHeight <<endl;


    return 0;



}