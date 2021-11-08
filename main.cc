#include <stdio.h>
#include "ringbuffer.h"

int main(int argc, char **argv)
{
    uint8_t data[] = {0x02, 0x03};
    uint8_t data1[] = {0, 0};
    ringbuffer_t *rb = ringbuffer_init(1024);
    printf("ava data:%d\n", ringbuffer_available(rb));
    ringbuffer_insert(rb,data,2);
    printf("ava data:%d\n", ringbuffer_available(rb));
    ringbuffer_pop(rb,data1,1);

    printf("data : %d", data1[0]);

    return 0;
}
