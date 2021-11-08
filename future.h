#pragma once

typedef struct future_t future_t;

#define FUTURE_SUCCESS ((void*)1)
#define FUTURE_FAIL ((void*)0)

// Constructs a new future_t object. Returns NULL on failure.
future_t* future_new(void);

// Constructs a new future_t object with an immediate |value|. No waiting will
// occur in the call to |future_await| because the value is already present.
// Returns NULL on failure.
future_t* future_new_immediate(void* value);

// Signals that the |future| is ready, passing |value| back to the context
// waiting for the result. Must only be called once for every future.
// |future| may not be NULL.
void future_ready(future_t* future, void* value);

// Waits for the |future| to be ready. Returns the value set in |future_ready|.
// Frees the future before return. |future| may not be NULL.
void* future_await(future_t* async_result);
