#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

// Inicializando as variáveis
static char buf[512] = { '\0' };
static char *p = buf;

// Defininfo a função recursiva
void find(char *path, char *name) {
  int fd;
  struct dirent de;
  struct stat st;

  // Abrindo o arquivo, ou ERR
  fd = open(path, 0);
  if (fd < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    exit(1);
  }

  // Passando as informações do arquivo aberto para um struct do tipo stat, ou ERR
  // Se OK, 'st' agora contem as informações acerca do arquivo aberto
  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    exit(1);
  }
  
  // Atribui '/' para 'p' (que aponta para buf[0]), move o ponteiro uma casa adiante.
  // Notar que no main já foi atribido um '.' para a referencia de p, então aqui, inicialmente, 'p' apontará para {'.', '/'}
  *p++ = '/';

  // Lê um arquivo para um struct do tipo dirent
  // Qual a condição de parada desse loop exatamente?
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    // (de.inum == 0) significa um diretório vazio
    if (de.inum == 0)
      continue;
    
    // Copia o nome do arquivo aberto para o 'buf' (para onde 'p' aponta)
    // Agora, 'p' vai ser './{de.name}'
    memcpy(p, de.name, DIRSIZ);

    // Se o nome do arquivo analisado pelo programa for igual ao parametro de busca 'name', é um match: printar no console
    if (!strcmp(p, name))
      printf("%s\n", buf);

    // Copiando as informações do arquivo aberto (reconhecido pelo nome armazenado em buf) 
    // para uma struct do tipo stat, ou ERR
    if (stat(buf, &st) < 0) {
      fprintf(2, "find: cannot stat %s\n", buf);
      exit(1);
    }

    // RECURSÃO
    // recursão nos subdiretorios, excluindo '.' e '..'
    // se o arquivo for do tipo diretorio e seu nome
    // não for '.' nem '..'
    if (strcmp(p, ".") && strcmp(p, "..") && st.type == T_DIR) {
      // Move o ponteiro strlen(de.name) casas adiante
      p += strlen(de.name);
      // Chamada recursiva, procure no diretorio cujo nome é o conteúdo de buf pelo nome passado como parametro para find.
      find(buf, name);
      // Quando sair da recursão, mover o ponteiro strlen(de.name) atrás
      p -= strlen(de.name);
    }

    // Preenche os 14 primeiros valores em buf com '\0'
    memset(p, '\0', DIRSIZ);
  }

  // Atribui o '\0' para onde 'p' está apontando, e move o ponteiro uma casa atrás
  *p-- = '\0';
  return;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(2, "usage: find <dirpath> <name>\n");
    exit(1);
  }

  char *path = argv[1];
  char *name = argv[2];

  // Atribui para 'p' (buf[0]) o char '.' e move o ponteiro adiante
  *p++ = '.';

  find(path, name);

  exit(0);
}
