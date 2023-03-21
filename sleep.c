#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int time_to_sleep;
    time_to_sleep = atoi(argv[1]);
    if(argc < 2) {
        write(1, "Erro! Sleep precisa de um argumento e nenhum foi fornecido.", 1);
        exit(1);
    } else {
        sleep(time_to_sleep);
    }
    exit(0);
}
