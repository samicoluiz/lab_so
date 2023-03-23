#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int pid;
    int p[2];
    char buf[10];
    pipe(p);
    pid = fork();
    
    write(p[1], "ping", sizeof("ping"));

    if(pid == 0) {
        char sendPong[] = "pong";
        read(p[0], buf, sizeof(buf));
        printf("%d: received %s\n",getpid(),buf);
        close(p[0]);
        write(p[1], sendPong, sizeof(sendPong));
        close(p[1]);
        exit(0);
        
    } else {
        wait(0);
        // char pong[] = "received pong";
        close(p[1]);
        read(p[0], buf, sizeof(buf));
        printf("%d: received %s\n",getpid(),buf);
        close(p[0]);
        exit(0);
    }

    exit(0);
}