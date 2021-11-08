#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#include "future.h"
#include "semaphore.h"

struct future_t {
  bool ready_can_be_called;
  semaphore_t* semaphore;  // NULL semaphore means immediate future
  void* result;
};

static void future_free(future_t* future);

future_t* future_new(void) {
  future_t* ret = static_cast<future_t*>(malloc(sizeof(future_t)));

  ret->semaphore = semaphore_new(0);
  if (!ret->semaphore) {
    printf("%s unable to allocate memory for the semaphore.",
              __func__);
    goto error;
  }

  ret->ready_can_be_called = true;
  return ret;
error:;
  future_free(ret);
  return NULL;
}

future_t* future_new_immediate(void* value) {
  future_t* ret = static_cast<future_t*>(malloc(sizeof(future_t)));

  ret->result = value;
  ret->ready_can_be_called = false;
  return ret;
}

void future_ready(future_t* future, void* value) {
  assert(future != NULL);
  assert(future->ready_can_be_called);

  future->ready_can_be_called = false;
  future->result = value;
  semaphore_post(future->semaphore);
}

void* future_await(future_t* future) {
  assert(future != NULL);

  // If the future is immediate, it will not have a semaphore
  if (future->semaphore) semaphore_wait(future->semaphore);

  void* result = future->result;
  future_free(future);
  return result;
}

static void future_free(future_t* future) {
  if (!future) return;

  semaphore_free(future->semaphore);
  free(future);
}

#ifdef TEST
future_t* future;

void *test(void *){

    char* data = (char *)future_await(future);
    printf("aaaa - 1\n");
    printf("value : %x\n",*data);
}

int main(int argc, char **argv){

    pthread_t id;

    char *data = (char *)malloc(1);

    *data = 0x04;
    
    future = future_new();

    int res = pthread_create(&id, NULL, test, NULL);

    printf("aaaa ---2\n");
    sleep(5);
    future_ready(future, data);
    printf("aaaa ---3\n");
    sleep(1);
    return 0;
}
#endif //TEST