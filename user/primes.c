#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int agrc, char *agrv[]) {
    int p[2];
    int prime[35], newprime[35];    // [1...[0]]: current primes
    pipe(p);
    prime[0] = 34;
    for(int i = 2; i <= 35; i++) {
        prime[i - 1] = i;           // init
    }
    int pid = fork();
    while(1) {
        if(pid < 0) {
            fprintf(2, "error: fail to fork!\n");
            exit(1);
        }
        else if(pid > 0) {          //parent
            printf("prime %d\n", prime[1]);
            close(p[0]);            // read close
            newprime[0] = 0;        // count new primes
            for(int i = 2; i <= prime[0]; i++) {
                if(prime[i] % prime[1] != 0) {
                    newprime[++newprime[0]] = prime[i];
                }
            }
            write(p[1], newprime, sizeof(int) * (newprime[0] + 1));
            close(p[1]);
            wait(0);
            exit(0);
        }
        else {
            close(p[1]);            // close write
            read(p[0], prime, sizeof(int));
            if(prime[0] == 0) {
                exit(0);            // finish: no more primes
            }
            read(p[0], prime + 1, sizeof(int) * prime[0]);
            close(p[0]);
            pipe(p);
            pid = fork();
        }
    }
    exit(0);
}