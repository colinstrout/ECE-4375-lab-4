#ifndef EX_BUFF_H_
#define EX_BUFF_H_

#include <pthread.h>
#include <stdio.h>

#define BUFFER_SIZE 16
struct element {
  int val;
  char c;
};

struct buffer {
  struct element buf[BUFFER_SIZE];
  int readPtr;
  int writePtr;
  int count;
  pthread_mutex_t mutex;
  pthread_cond_t condition;
};

void buffer_init(struct buffer *b);
void enqueue(struct buffer *b, char name, int value);
void dequeue(struct buffer *b, char *name, int *value);
void buffer_destroy(struct buffer *b);
#endif // EX_BUFF_H_