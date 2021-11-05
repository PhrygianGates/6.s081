#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "usage: xargs <command>");
        exit(1);
    }
    char *_argv[MAXARG];
    int i = 0;
    for (; i < argc - 1; i++) {
        _argv[i] = argv[i + 1];
    }
    char buf[1024];
    char *p = buf, *q = buf;
    char c;
    while(1) {
        if (read(0, &c, 1) != 1) {
            break;
        }
        if (c == '\n' || c == ' ') {
            *q = 0;
            _argv[i] = p;
            i++;
            q++;
            p = q;
        } else {
            *q = c;
            q++;
        }
    }
    if (fork() == 0) {
        exec(_argv[0], _argv);
    } else {
        wait(0);
    }
    exit(0);
}