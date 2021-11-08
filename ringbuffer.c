#include <stdlib.h>
#include "ringbuffer.h"

struct ringbuffer_t {
  size_t total;
  size_t available;
  uint8_t* base;
  uint8_t* head;
  uint8_t* tail;
};


ringbuffer_t* ringbuffer_init(const size_t size) {
  ringbuffer_t* p =
      static_cast<ringbuffer_t*>(osi_calloc(sizeof(ringbuffer_t)));

  p->base = static_cast<uint8_t*>(osi_calloc(size));
  p->head = p->tail = p->base;
  p->total = p->available = size;

  return p;
}
