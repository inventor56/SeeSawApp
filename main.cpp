#include <iostream>
#include <semaphore.h>

using namespace std;

#define NUM_THREADS 2 // Two threads in this program


void *fredSee(void*) { // Fred's Behavior


    pthread_exit(nullptr);

}
void *wilmaSaw(void*) { // Wilma's Behavior


    pthread_exit(nullptr);
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    pthread_t threads[NUM_THREADS];

    // First, we list our two sempahores
    sem_t increaseHeight; // Create semaphore increaseHeight
    sem_t decreaseHeight; // Create semaphore decreaseHeight

    sem_init(&increaseHeight, 1, 1); // Initialize increaseHeight semaphore, shared between threads, and initialize value to 1
    sem_init(&decreaseHeight, 1, 1); // Initialize decreaseHeight semaphore, shared between threads, and initialize value to 1

    int tid;

    // Create Threads

    // Thread A (Fred)
    tid = pthread_create(&threads[0], nullptr, fredSee, nullptr); // Thread for Fred
    // Check for threading errors
    if (tid) {
        cout << "Error: Problem creating thread," << tid << endl;
        exit(-1);
    }

    // Thread B (Wilma)
    tid = pthread_create(&threads[1], nullptr, wilmaSaw, nullptr); // Thread for Wilma
    // Check for threading errors
    if (tid) {
        cout << "Error: Problem creating thread," << tid << endl;
        exit(-1);
    }


    // Join all threads: Making sure that all have completed before continuing onwards
    for (unsigned long thread : threads) {
        pthread_join(thread, nullptr); // Join threads
    }




    return 0;



}