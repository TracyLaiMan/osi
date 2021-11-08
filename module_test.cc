#include <stdio.h>
#include <dlfcn.h>

#include "module.h"
#include "dummy_module.h"

int main(int argc, char **argv){
    
    // Always try to load the default Bluetooth stack on GN builds.
    void* handle = dlopen("libmodule.so", RTLD_NOW);

    printf("start test!\n");
    module_start_up(get_module(DUMMY_MODULE));
    return 0;
}
