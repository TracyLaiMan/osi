#include <dlfcn.h>
#include <string.h>
#include <assert.h>
#include <mutex>
#include <unordered_map>

#include "module.h"

typedef enum {
  MODULE_STATE_NONE = 0,
  MODULE_STATE_INITIALIZED = 1,
  MODULE_STATE_STARTED = 2
} module_state_t;

static std::unordered_map<const module_t*, module_state_t> metadata;

// TODO(jamuraa): remove this lock after the startup sequence is clean
static std::mutex metadata_mutex;

static bool call_lifecycle_function(module_lifecycle_fn function);
static module_state_t get_module_state(const module_t* module);
static void set_module_state(const module_t* module, module_state_t state);

void module_management_start(void) {}

void module_management_stop(void) {
  metadata.clear();
}

const module_t* get_module(const char* name) {
  module_t* module = (module_t*)dlsym(RTLD_DEFAULT, name);
  assert(module);
  return module;
}

bool module_init(const module_t* module) {
  assert(module != NULL);
  assert(get_module_state(module) == MODULE_STATE_NONE);

  if (!call_lifecycle_function(module->init)) {
    printf("%s Failed to initialize module \"%s\" \n", __func__,
              module->name);
    return false;
  }

  set_module_state(module, MODULE_STATE_INITIALIZED);
  return true;
}

bool module_start_up(const module_t* module) {
  assert(module != NULL);
  // TODO(zachoverflow): remove module->init check once automagic order/call is
  // in place.
  // This hack is here so modules which don't require init don't have to have
  // useless calls
  // as we're converting the startup sequence.
  assert(get_module_state(module) == MODULE_STATE_INITIALIZED ||
        module->init == NULL);

  printf("%s Starting module \"%s\" \n", __func__, module->name);
  if (!call_lifecycle_function(module->start_up)) {
    printf("%s Failed to start up module \"%s\" \n", __func__,
              module->name);
    return false;
  }
  printf("%s Started module \"%s\" \n", __func__, module->name);

  set_module_state(module, MODULE_STATE_STARTED);
  return true;
}

void module_shut_down(const module_t* module) {
  assert(module != NULL);
  module_state_t state = get_module_state(module);
  assert(state <= MODULE_STATE_STARTED);

  // Only something to do if the module was actually started
  if (state < MODULE_STATE_STARTED) return;

  printf("%s Shutting down module \"%s\"", __func__, module->name);
  if (!call_lifecycle_function(module->shut_down)) {
    printf("%s Failed to shutdown module \"%s\". Continuing anyway.",
              __func__, module->name);
  }
  printf("%s Shutdown of module \"%s\" completed", __func__,
           module->name);

  set_module_state(module, MODULE_STATE_INITIALIZED);
}

void module_clean_up(const module_t* module) {
  assert(module != NULL);
  module_state_t state = get_module_state(module);
  assert(state <= MODULE_STATE_INITIALIZED);

  // Only something to do if the module was actually initialized
  if (state < MODULE_STATE_INITIALIZED) return;

  printf("%s Cleaning up module \"%s\"", __func__, module->name);
  if (!call_lifecycle_function(module->clean_up)) {
      printf("%s Failed to cleanup module \"%s\". Continuing anyway.",
              __func__, module->name);
  }
  printf("%s Cleanup of module \"%s\" completed", __func__,
           module->name);

  set_module_state(module, MODULE_STATE_NONE);
}

static bool call_lifecycle_function(module_lifecycle_fn function) {
  // A NULL lifecycle function means it isn't needed, so assume success
  if (!function) return true;

  future_t* future = function();

  // A NULL future means synchronous success
  if (!future) return true;

  // Otherwise fall back to the future
  return future_await(future);
}

static module_state_t get_module_state(const module_t* module) {
  std::lock_guard<std::mutex> lock(metadata_mutex);
  auto map_ptr = metadata.find(module);

  return (map_ptr != metadata.end()) ? map_ptr->second : MODULE_STATE_NONE;
}

static void set_module_state(const module_t* module, module_state_t state) {
  std::lock_guard<std::mutex> lock(metadata_mutex);
  metadata[module] = state;
}
