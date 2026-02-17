#include "ex_buff.h"
#include <stdint.h>

void *bodyProducer(void *p) {
  struct buffer *buf = (struct buffer *)p;
  for (uint16_t i = 1; i <= 1000; ++i) {
    printf("%c: Enqueueing %d into buffer\n", 'P', i);
    enqueue(buf, 'P', i);

    fflush(stdout);
  }
  return NULL;
}

void *bodyConsumer(void *p) {
  struct buffer *buf = (struct buffer *)p;
  for (uint16_t i = 1; i <= 1000; ++i) {
    char name;
    int val;

    dequeue(buf, &name, &val);

    printf("Dequeued %c, %d from buffer\n", name, val);
    fflush(stdout);
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  struct buffer buf;
  buffer_init(&buf);

  pthread_t prod, con;

  pthread_create(&prod, NULL, bodyProducer, &buf);
  pthread_create(&con, NULL, bodyConsumer, &buf);

  pthread_join(prod, NULL);
  pthread_join(con, NULL);

  buffer_destroy(&buf);
  return 0;
}