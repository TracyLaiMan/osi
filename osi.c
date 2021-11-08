#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

#define INVALID_FD (-1)
#define RANDOM_PATH "/dev/urandom"

int osi_rand(void){
    int rand;
    int rand_fd = open(RANDOM_PATH, O_RDONLY);
    if(rand_fd == INVALID_FD){
        assert(rand_fd != INVALID_FD);
    }

    ssize_t read_bytes = read(rand_fd, &rand, sizeof(rand));
    close(rand_fd);

    assert(read_bytes == sizeof(rand));

    if(rand < 0) rand = -rand;

    return rand;

}

int main(int argc, char **argv){

    printf("rand value : %d", osi_rand());

    return 0;
}
