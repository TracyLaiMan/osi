#include <stdio.h>
#include "module.h"
#include "dummy_module.h"

static future_t* dummy_module_start_up(void) {
    printf("%s", __func__);
    return NULL;
}

__attribute__((visibility("default"))) extern const module_t dummy_module = {
    .name = DUMMY_MODULE,
    .init = NULL,
    .start_up = dummy_module_start_up,
    .shut_down = NULL,
    .clean_up = NULL,
    .dependencies = {NULL}};


