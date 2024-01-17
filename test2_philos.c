#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NUM_PHILOSOPHERS 5
#define NUM_CHOPSTICKS 5

void *dine(void *n);
pthread_t philosopher[NUM_PHILOSOPHERS];
pthread_mutex_t chopstick[NUM_CHOPSTICKS];

int main()
{
  // Define counter var i and status_message
  int i, status_message;
  void *msg;

  // Initialise the semaphore array
  for (i = 0; i < NUM_CHOPSTICKS; i++)
  {
    status_message = pthread_mutex_init(&chopstick[i], NULL);
    // Check if the mutex is initialised successfully
    if (status_message == -1)
    {
      printf("\n Mutex initialization failed");
      exit(1);
    }
  }

  // Run the philosopher Threads using *dine() function
  for (i = 1; i <= NUM_PHILOSOPHERS; i++)
  {
    status_message = pthread_create(&philosopher[i -1], NULL, (void *)dine, (void *)i);
    usleep(1);
    if (status_message != 0)
    {
      printf("\n Thread creation error \n");
      exit(1);
    }
  }

  // Wait for all philosophers threads to complete executing (finish dining) before closing the program
  for (i = 0; i < NUM_PHILOSOPHERS; i++)
  {
    status_message = pthread_join(philosopher[i], &msg);
    if (status_message != 0)
    {
      printf("\n Thread join failed for thread %d with error code %d\n", i, status_message);
      exit(1);
    }
  }

  // Destroy the chopstick Mutex array
  for (i = 0; i < NUM_CHOPSTICKS; i++)
  {
    status_message = pthread_mutex_destroy(&chopstick[i]);
    if (status_message != 0)
    {
      printf("\n Mutex not destroyed \n");
      exit(1);
    }
  }
  return 0;
}

void *dine(void *arg)
{
  int n = (int)arg;
  printf("Philosopher %d is thinking\n", n);

  // Philosopher picks up the left chopstick (wait)
  pthread_mutex_lock(&chopstick[n]);
  printf("Philosopher %d picked up left chopstick\n", n);

  // Philosopher picks up the right chopstick (wait)
  pthread_mutex_lock(&chopstick[(n + 1) % NUM_CHOPSTICKS]);
  printf("Philosopher %d picked up right chopstick\n", n);

  // After picking up both the chopsticks, philosopher starts eating
  printf("Philosopher %d is eating\n", n);
  sleep(3);

  // Philosopher places down the left chopstick (signal)
  pthread_mutex_unlock(&chopstick[n]);
  printf("Philosopher %d placed down left chopstick\n", n);

  // Philosopher places down the right chopstick (signal)
  pthread_mutex_unlock(&chopstick[(n + 1) % NUM_CHOPSTICKS]);
  printf("Philosopher %d placed down right chopstick\n", n);

  // Philosopher finishes eating
  printf("Philosopher %d finished eating\n", n);
}