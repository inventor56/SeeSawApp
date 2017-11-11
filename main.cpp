#include <iostream>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define NUM_THREADS 2 // Two threads in this program
#define MIN_HEIGHT 1 // Minimum height a person can be at
#define MAX_HEIGHT 7 // Maximum height a person can be at

pthread_t threads[NUM_THREADS];

// First, we list our two sempahores
sem_t fredBehavior; // Create semaphore fredBehavior
sem_t wilmaBehavior; // Create semaphore wilmaBehavior

int upDown = 0;

// Store velocity, will change depending on which person is "kicking off" (i.e Fred is at 1 ft/sec, Wilma is at 1.5 ft/sec)
float velocity;

// We'll store the heights for each person here
float fredHeight = 1; // Fred (Person A) starts on the low end, one foot off the ground
float wilmaHeight = 7; // Wilma (Person B) starts on the high end, seven feet off the ground

void *fredSee(void*) { // Fred's Behavior: Thread simulates what happens when Fred is moving upwards, and Wilma is moving downwards

    while(upDown < 9) { // Ensures that the simulation only runs 10 times ( up and down)
        sem_wait(&fredBehavior); // !!! KEY ASPECT !!!! - Semaphore ensures Fred will not push up unless Wilma has finished her behavior.
        velocity = 1; // Since this thread concerns Fred's behavior, his velocity is set to 1 ft/sec
        while(fredHeight < MAX_HEIGHT && wilmaHeight > MIN_HEIGHT) { // Fred pushes up while his height is not at 7 ft, and while Wilma's height is at 1 ft
            cout << "Fred Height: " << fredHeight << endl; // Print out Fred's current height
            cout << "Wilma Height: " << wilmaHeight << endl << "\n"; // Print out Wilma's current height
            fredHeight += velocity; // Fred's height increases by the current velocity (1)
            wilmaHeight -= velocity; // Wilma's height decreases by the current velocity (1)
            sleep(1); // Thread will be throttled by 1 second (for terminal output)

        }
        sem_post(&wilmaBehavior); // Signal Wilma to begin her behavior (Fred has returned to within 1 ft of the ground)
    }

    pthread_exit(nullptr); // Exit thread at end of simulation

}
void *wilmaSaw(void*) { // Wilma's Behavior

    while(upDown < 10) { // Wilma's Behavior: Thread simulates what happens when Wilma is moving upwards, and Fred is moving downwards
        sem_wait(&wilmaBehavior); // !!! KEY ASPECT !!!! - Semaphore ensures Wilma will not push up unless Fred has finished his behavior.
        velocity = 1.5; // Since this thread concerns Wilma's behavior, her velocity is set to 1.5 ft/sec
        while(wilmaHeight < MAX_HEIGHT && fredHeight > MIN_HEIGHT) { // Wilma pushes up while her height is not at 7 ft, and while Fred's height is at 1 ft
            cout << "Fred Height: " << fredHeight << endl; // Print out Fred's current height
            cout << "Wilma Height: " << wilmaHeight << endl << "\n"; //Print out Wilma's current height
            wilmaHeight += velocity; // Wilma's height increases by the current velocity (1.5)
            fredHeight -= velocity;  // Fred's height decreases by the current velocity (1.5)
            sleep(1); // Thread will be throttled by 1 second (for terminal output)

        }
        upDown++; // Since Fred starts the simulation, once Wilma comes down a full up-down has been been performed. Increment by 1
        cout << "Iteration: " << upDown << " Complete\n\n"; // Print out the current iteration for user readability
        sem_post(&fredBehavior); // Signal Fred to begin his behavior (Wilma has returned to within 1 ft of the ground)
    }

    pthread_exit(nullptr); // Exit thread at end of simulation
}

void checkForThreadErrors(int tid) { // Error checking thread to ensure threads were created successfully
    if (tid) {
        cout << "Error: Problem creating thread," << tid << endl;
        exit(-1);
    }
}

int main(int argc, char** argv) {

    sem_init(&fredBehavior, 1, 1); // Initialize fredBehavior semaphore, shared between threads, and initialize value to 1
    sem_init(&wilmaBehavior, 1, 0); // Initialize wilmaBehavior semaphore, shared between threads, and initialize value to 0

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
    cout << "Finished with Fred Height: " << fredHeight << ", Wilma Height: " << wilmaHeight <<endl;

    return 0;
}