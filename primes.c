#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// logica do filtro de eratostenes
void filtro(int read_end) {
    int p; // variavel para armazenar o primo
    // se a leitura retornar 0 (chegou ao fim), executa a condicional
    if (!read(read_end, &p, sizeof(int))) {
        // fecha o read end
        close(read_end);
        // encerra o processo
        exit(0);
    }
    // printa o primo p
    printf("prime %d\n", p);

    // criando o array com os espaços para os file descriptors do pipe
    int fds[2];
    // criando o pipe
    pipe(fds);
    // fazendo o fork
    int pid = fork();
    if (pid == 0) { // child
        // fecha o write end
        close(fds[1]);
        // chamada recursiva da funcao no read end do pipe
        filtro(fds[0]);
        // fecha o read end do pipe
        close(fds[0]);
        // encerra o processo filho
        exit(0);
    } else if (pid > 0) { //pai
        close(fds[0]);
    }
    
    // checando a divisibilidade
    int n;
    // para cada numero que passar no pipe
    while (read(read_end, &n, sizeof(int))) {
        // se n dividido por p for diferente de 0 (n não é divisível por p), executar o bloco
        if (n % p != 0) {
            write(fds[1], &n, sizeof(int));
        }        
    }
    // fecha o write end do pipe
    close(fds[1]);
    // espera o encerramento dos processos filhos
    wait(0);
    // encerra o processo pai
    exit(0);    
}


int main(int argc, char *argv[]) {
    // criando o array com os espaços para os file descriptors do pipe
    int fds[2];
    // abrindo o pipe
    pipe(fds);
    // fazendo o fork
    int pid = fork();

    // aplicando o filtro de eratostenes
    if (pid == 0) { // se child
        // fecha o write end
        close(fds[1]); 
        // aplica o filtro no valor contido no read end do pipe
        filtro(fds[0]);
        // fecha o read end do pipe
        close(fds[0]);
        // encerra o processo filho
        exit(0);
    } else if (pid > 0) { // maior que 0 porque -1 é erro
        // fecha o read end do pipe
        close(fds[0]);
        // envia os numeros de 2 (primeiro primo) a 35 para os processos filhos
        for (int i = 2; i <= 35; i++) {
            // escreve o numero no write end do pipe
            write(fds[1], &i, sizeof(int));            
        }
        // fecha o write end do pipe
        close(fds[1]);
        // espera o encerramento dos processos filho
        wait(0);
        // encerra o processo pai
        exit(0);
    }
    exit(0);
}
