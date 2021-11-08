#pragma once

#include <stdbool.h>

#include "future.h"

typedef future_t* (*module_lifecycle_fn)(void);

#define BTCORE_MAX_MODULE_DEPENDENCIES 10

typedef struct {
  const char* name;
  module_lifecycle_fn init;
  module_lifecycle_fn start_up;
  module_lifecycle_fn shut_down;
  module_lifecycle_fn clean_up;
  const char* dependencies[BTCORE_MAX_MODULE_DEPENDENCIES];
} module_t;

// Prepares module management. Must be called before doing anything with
// modules.
void module_management_start(void);
// Cleans up all module management resources.
void module_management_stop(void);

const module_t* get_module(const char* name);

// Initialize the provided module. |module| may not be NULL
// and must not be initialized.
bool module_init(const module_t* module);
// Start up the provided module. |module| may not be NULL
// and must be initialized or have no init function.
bool module_start_up(const module_t* module);
// Shut down the provided module. |module| may not be NULL.
// If not started, does nothing.
void module_shut_down(const module_t* module);
// Clean up the provided module. |module| may not be NULL.
// If not initialized, does nothing.
void module_clean_up(const module_t* module);