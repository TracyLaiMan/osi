#include "semaphore.h"
#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#define INVALID_FD (-1)

struct semaphore_t {
  int fd;
};

semaphore_t* semaphore_new(unsigned int value) {
  semaphore_t* ret = static_cast<semaphore_t*>(malloc(sizeof(semaphore_t)));
  ret->fd = eventfd(value, EFD_SEMAPHORE);
  if (ret->fd == INVALID_FD) {
    free(ret);
    ret = NULL;
  }
  return ret;
}

void semaphore_free(semaphore_t* semaphore) {
  if (!semaphore) return;

  if (semaphore->fd != INVALID_FD) close(semaphore->fd);
  free(semaphore);
}

void semaphore_wait(semaphore_t* semaphore) {
  assert(semaphore != NULL);
  assert(semaphore->fd != INVALID_FD);

  eventfd_t value;
  if (eventfd_read(semaphore->fd, &value) == -1)
    printf("%s unable to wait on semaphore: %s", __func__,
              strerror(errno));
}


bool semaphore_try_wait(semaphore_t* semaphore) {
  assert(semaphore != NULL);
  assert(semaphore->fd != INVALID_FD);

  int flags = fcntl(semaphore->fd, F_GETFL);
  if (flags == -1) {
    printf("%s unable to get flags for semaphore fd: %s", __func__,
              strerror(errno));
    return false;
  }
  if (fcntl(semaphore->fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    printf("%s unable to set O_NONBLOCK for semaphore fd: %s",
              __func__, strerror(errno));
    return false;
  }

  bool rc = true;
  eventfd_t value;
  if (eventfd_read(semaphore->fd, &value) == -1) rc = false;

  if (fcntl(semaphore->fd, F_SETFL, flags) == -1)
    printf("%s unable to restore flags for semaphore fd: %s",
              __func__, strerror(errno));
  return rc;
}

void semaphore_post(semaphore_t* semaphore) {
  assert(semaphore != NULL);
  assert(semaphore->fd != INVALID_FD);

  if (eventfd_write(semaphore->fd, 1ULL) == -1)
    printf("%s unable to post to semaphore: %s", __func__,
              strerror(errno));
}

int semaphore_get_fd(const semaphore_t* semaphore) {
  assert(semaphore != NULL);
  assert(semaphore->fd != INVALID_FD);
  return semaphore->fd;
}


#ifdef TEST
#include <pthread.h>

semaphore_t* semaphore;

void *test(void *){

    semaphore_wait(semaphore);
    printf("aaaa - 1\n");
}

int main(int argc, char **argv){

    pthread_t id;
    
    semaphore = semaphore_new(0);

    int res = pthread_create(&id, NULL, test, NULL);

    printf("aaaa ---2\n");
    sleep(5);
    semaphore_post(semaphore);
    printf("aaaa ---3\n");
    sleep(1);
    return 0;
}
#endif //TEST

