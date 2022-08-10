#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"
int main(int argc, char* argv[]) {
    char* newargv[MAXARG];      // args for exec
    char** lastpos = newargv;   // arg_last_pos
    for (int i = 1; i < argc; i++, lastpos++)
        *lastpos = argv[i];     // point to argv
    char** argv_cur = lastpos;  // fill argv
    char buf[MAXARG][100];      // store args from stdin
    memset(buf, 0, MAXARG * 100);
    int i = 0, j = 0;
    while (read(0, buf[i] + j, 1) > 0) {
        if (buf[i][j] == ' ') { // end of a word
            buf[i][j] = 0;
            *argv_cur = buf[i];
            argv_cur++;
            i++, j = 0;
        }
        else if (buf[i][j] == '\n') {
            buf[i][j] = 0;
            *argv_cur = buf[i];
            i = j = 0;
            int pid = fork();
            if (pid == 0) {     // children
                argv_cur++;
                **argv_cur = 0;
                exec(argv[1], newargv);
                exit(0);
            }
            else {              // parent
                wait(0);
                argv_cur = lastpos;
            }
        }
        else
            j++;                // next char 
    }
    exit(0);
}