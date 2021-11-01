#include "kernel/types.h"
#include "user/user.h"

int main() {
    int p[2];
    int pid;
    pipe(p);
    if (fork() == 0) {
        pid = getpid();
        char buf[5];
        char info[] = "pong";
        if (read(p[0], buf, 5) != 5) {
            fprintf(2, "child read error");
            exit(1);
        }
        close(p[0]);
        fprintf(1, "%d: received %s\n", pid, buf);
        if (write(p[1], info, 5) != 5) {
            fprintf(2, "child write error");
            exit(1);
        }
        close(p[1]);
        exit(0);
    } else {
        pid = getpid();
        char buf[5];
        char info[] = "ping";
        if (write(p[1], info, 5) != 5) {
            fprintf(2, "parent write error");
            exit(1);
        }
        close(p[1]);
        wait(0);
        if (read(p[0], buf, 5) != 5) {
            fprintf(2, "parent read error");
            exit(1);
        }
        close(p[0]);
        fprintf(1, "%d: received %s\n", pid, buf);
        exit(0);
    }
}