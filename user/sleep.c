#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int agrc, char *agrv[]){
    if(agrc < 2) {
        fprintf(2, "error: sleep time is required!\n");
        exit(1);
    }
    int sleep_time = atoi(agrv[1]);
    sleep(sleep_time);
    exit(0);
}