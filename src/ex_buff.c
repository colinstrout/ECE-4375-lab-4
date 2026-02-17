#include "ex_buff.h"

void buffer_init(struct buffer *b) {
  b->readPtr = 0;
  b->writePtr = 0;
  b->count = 0;

  if (pthread_mutex_init(&(b->mutex), NULL) != 0) {
    perror("[ERROR] Error initializing mutex");
  }

  if (pthread_cond_init(&(b->condition), NULL) != 0) {
    perror("[ERROR] Condition Variable Initializer Failed");
    pthread_mutex_destroy(&(b->mutex));
  }
}

void enqueue(struct buffer *b, char name, int value) {
  pthread_mutex_lock(&(b->mutex));

  while (b->count == BUFFER_SIZE) {
    pthread_cond_wait(&(b->condition), &(b->mutex));
  }
  struct element new_element = {value, name};

  b->buf[b->writePtr] = new_element;
  b->writePtr = (b->writePtr + 1) % BUFFER_SIZE;
  ++(b->count);

  pthread_cond_broadcast(&(b->condition));
  pthread_mutex_unlock(&(b->mutex));
}

void dequeue(struct buffer *b, char *name, int *value) {
  pthread_mutex_lock(&(b->mutex));

  while (b->count == 0) {
    pthread_cond_wait(&(b->condition), &(b->mutex));
  }

  struct element item = b->buf[b->readPtr];
  *name = item.c;
  *value = item.val;

  b->readPtr = (b->readPtr + 1) % BUFFER_SIZE;
  --(b->count);

  pthread_cond_broadcast(&(b->condition));
  pthread_mutex_unlock(&(b->mutex));
}

void buffer_destroy(struct buffer *b) {
  pthread_mutex_destroy(&(b->mutex));
  pthread_cond_destroy(&(b->condition));
}