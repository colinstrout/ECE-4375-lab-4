#include "ex_buff.h"
#include <stdint.h>

struct threadArgs {
  struct buffer *buf;
  char id;
};

void *bodyProducer(void *p) {
  struct threadArgs *targs = (struct threadArgs *)p;

  struct buffer *buf = targs->buf;
  char name = targs->id;
  for (uint16_t i = 1; i <= 1000; ++i) {
    // printf("%c: Enqueueing %d into buffer\n", name, i);
    enqueue(buf, name, i);
    // fflush(stdout);
  }
  return NULL;
}

void *bodyConsumer(void *p) {
  struct buffer *buf = (struct buffer *)p;

  int next_expected[3] = {1, 1, 1};
  int total_received = 0;
  int errors = 0;

  for (uint32_t i = 0; i < 3000; ++i) {
    char name;
    int val;

    dequeue(buf, &name, &val);
    total_received++;

    int idx = name - 'A';

    if (val != next_expected[idx]) {
      printf("ORDER ERROR: Producer %c sent %d, but expected %d\n", name, val,
             next_expected[idx]);
      errors++;
    }

    next_expected[idx] = val + 1;
  }

  printf("\n--- Test Results ---\n");
  printf("Total Messages Received: %d / 3000\n", total_received);

  if (errors == 0 && total_received == 3000) {
    printf("SUCCESS: All data received in perfect order!\n");
  } else {
    printf("FAILURE: Found %d ordering errors.\n", errors);
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  struct buffer buf;
  buffer_init(&buf);
  uint8_t num_producers = 3;
  pthread_t prods[num_producers];
  pthread_t con;
  struct threadArgs a[num_producers];
  char names[] = {'A', 'B', 'C'};

  for (uint8_t i = 0; i < num_producers; ++i) {
    a[i].buf = &buf;
    a[i].id = names[i];

    pthread_create(&prods[i], NULL, bodyProducer, &a[i]);
  }
  pthread_create(&con, NULL, bodyConsumer, &buf);

  for (uint8_t i = 0; i < num_producers; ++i) {
    pthread_join(prods[i], NULL);
  }
  pthread_join(con, NULL);

  buffer_destroy(&buf);
  return 0;
}