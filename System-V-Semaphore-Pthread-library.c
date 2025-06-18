#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>

#define TOTAL_PHILOSOPHERS 5
#define SHARED_KEY 9876

// Union for semctl operatiosn
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *_buf;
};

int semaphore_id;

void *philosophersEatThink(void *arg) {
    int philosopher_id = *((int *)arg);
    int left_chopstick = philosopher_id;
    int right_chopstick = (philosopher_id + 1) % TOTAL_PHILOSOPHERS;
    struct sembuf actions[2];

    // Infinite loop
    while (1) {
        printf("Philosopher %d wants to eat.\n", philosopher_id);

        // Pick left chopstick
        actions[0].sem_num = left_chopstick;
        actions[0].sem_op = -1;
        actions[0].sem_flg = 0;
        
        // Pick right chopstick
        actions[1].sem_num = right_chopstick;
        actions[1].sem_op = -1;
        actions[1].sem_flg = 0;

        if (semop(semaphore_id, actions, 2) == -1) {
            perror("Chopstick acquisition failed");
            exit(1);
        }

	// Random time for eating
        int eating_duration = rand() % 3 + 2;
        printf("Philosopher %d is going to eat. \n", philosopher_id);
        sleep(eating_duration);

        // Release chopsticks
        actions[0].sem_op = 1;
        actions[1].sem_op = 1;

        if (semop(semaphore_id, actions, 2) == -1) {
            perror("Chopstick release failed");
            exit(1);
        }
        
        // Random time for thinking
        int thinking_duration = rand() % 3 + 1;
        printf("Philosopher %d finished eating, now going to think \n", philosopher_id);
        sleep(thinking_duration);
    }

    return NULL;
}

int main() {

    pthread_t philosophers[TOTAL_PHILOSOPHERS];
    int philosopher_ids[TOTAL_PHILOSOPHERS];

    // Creating semaphores
    semaphore_id = semget(SHARED_KEY, TOTAL_PHILOSOPHERS, IPC_CREAT | 0666);
    if (semaphore_id == -1) {
        perror("Failed to create chopstick semaphores");
        exit(1);
    }

    union semun sem_config;
    unsigned short initial_values[TOTAL_PHILOSOPHERS];

    for (int i = 0; i < TOTAL_PHILOSOPHERS; ++i)
        initial_values[i] = 1;

    sem_config.array = initial_values;

    if (semctl(semaphore_id, 0, SETALL, sem_config) == -1) {
        perror("Failed to initialize chopsticks");
        exit(1);
    }

    // Creating one thread for each philosopher
    for (int i = 0; i < TOTAL_PHILOSOPHERS; ++i) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosophersEatThink, &philosopher_ids[i]);
    }

    for (int i = 0; i < TOTAL_PHILOSOPHERS; ++i) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}

