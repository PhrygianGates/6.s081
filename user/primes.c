#include "kernel/types.h"
#include "user/user.h"

void redirect(int k, int pd[2]) {
    close(k);
    dup(pd[k]);
    close(pd[0]);
    close(pd[1]);
}

void proc_layer() {
    int p;
    if (read(0, &p, sizeof(p))) {
        printf("prime %d\n", p);
        int pd[2];
        pipe(pd);
        if (fork() == 0) {
            redirect(0, pd);
            proc_layer();
        } else {
            redirect(1, pd);
            int n;
            while (read(0, &n, sizeof(n))) {
                if (n % p) {
                    write(1, &n, sizeof(n));
                }
            }
            close(0);
            close(1);
            wait(0);
        }
    }
    exit(0);
}

int main() {
    int pd[2];
    pipe(pd);
    if (fork() == 0) {
        redirect(0, pd);
        proc_layer();
    } else {
        redirect(1, pd);
        for (int i = 2; i < 36; i++) {
            write(1, &i, sizeof(i));
        }
        close(1);
        wait(0);
        exit(0);
    }
    return 0;
}
