#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int agrc, char *agrv[]) {
    int p_cp[2], p_pc[2];
    pipe(p_cp);
    pipe(p_pc);
    char buf[2];
    int pid = fork();
    if(pid < 0) {
        fprintf(2, "error: fail to fork!\n");
        exit(1);
    }
    else if(pid > 0) {      // parent
        close(p_pc[0]);     // parent_to_child read
        close(p_cp[1]);     // child_to_parent write
        if(write(p_pc[1], "p", 1) != 1) {
            fprintf(2, "error: parent fail to write!\n");
            exit(1);
        }
        close(p_pc[1]);     // parent_to_child write
        if(read(p_cp[0], buf, 1) != 1) {
            fprintf(2, "error: parent fail to read!\n");
            exit(1);
        }
        close(p_cp[0]);     // child_to_parent read
        if(buf[0] == 'c') {
            printf("%d: received pong\n", getpid());
        }
        else {
            fprintf(2, "error: parent receives wrongly\n");
            exit(1);
        }
        exit(0);
    }
    else {  // child
        close(p_pc[1]);     // parent_to_child write
        close(p_cp[0]);     // child_to_parent read
        if(read(p_pc[0], buf, 1) != 1) {
            fprintf(2, "error: child fail to read!\n");
            exit(1);
        }
        close(p_pc[0]);     // parent_to_child read
        if(buf[0] == 'p') {
            printf("%d: received ping\n", getpid());
        }
        else {
            fprintf(2, "error: child receives wrongly\n");
            exit(1);
        }
        if(write(p_cp[1], "c", 1) != 1) {
            fprintf(2, "error: child fail to write!\n");
            exit(1);
        }
        close(p_cp[1]);     // child_to_parent write
        exit(0);
    }
}