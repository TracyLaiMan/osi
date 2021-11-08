#pragma once

#include <stdbool.h>

struct semaphore_t;
typedef struct semaphore_t semaphore_t;

// Creates a new semaphore with an initial value of |value|.
// Returns NULL on failure. The returned object must be released
// with |semaphore_free|.
semaphore_t* semaphore_new(unsigned int value);

// Frees a semaphore allocated with |semaphore_new|. |semaphore| may
// be NULL.
void semaphore_free(semaphore_t* semaphore);

// Decrements the value of |semaphore|. If it is 0, this call blocks until
// it becomes non-zero. |semaphore| may not be NULL.
void semaphore_wait(semaphore_t* semaphore);
#if defined(MTK_COMMON) && (MTK_COMMON == TRUE)
// Decrements the value of |semaphore|. If it is 0, this call blocks until
// timeout or it becomes non-zero. |semaphore| may not be NULL.
void semaphore_wait_timeout(semaphore_t* semaphore, long usec);
#endif
// Tries to decrement the value of |semaphore|. Returns true if the value was
// decremented, false if the value was 0. This function never blocks.
// |semaphore| may not be NULL.
bool semaphore_try_wait(semaphore_t* semaphore);

// Increments the value of |semaphore|. |semaphore| may not be NULL.
void semaphore_post(semaphore_t* semaphore);

// Returns a file descriptor representing this semaphore. The caller may
// only perform one operation on the file descriptor: select(2). If |select|
// indicates the fd is readable, the caller may call |semaphore_wait|
// without blocking. If select indicates the fd is writable, the caller may
// call |semaphore_post| without blocking. Note that there may be a race
// condition between calling |select| and |semaphore_wait| or |semaphore_post|
// which results in blocking behaviour.
//
// The caller must not close the returned file descriptor. |semaphore| may not
// be NULL.
int semaphore_get_fd(const semaphore_t* semaphore);
