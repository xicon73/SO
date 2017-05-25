#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /*File control -> O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define PIPE_BUF 100

ssize_t readLn(int fildes, char *buf, size_t nbyte, int* caracteresL) {
  int nCarateresL = 0;
  char *aux = buf;
  int i;
  // Lê o ficheiro enquando não for ultrapassado o numero de bytes.
  while((aux-buf < PIPE_BUF) && (i = read(fildes,aux, nbyte)) == 1) {
    nCarateresL++;
    // Se chega ao fim da linha/ficheiro
    if(*aux == '\n' || *aux == EOF) {
      aux++;
      *(--aux) = (char) NULL;
      // Número de bytes lidos
      return (aux - buf);
    }
    aux++;
  }
  if(i <= 0){
    *caracteresL = nCarateresL;
    return -1;
  }

  return (aux - buf);
}

int prefix(const char *command, const char *test) {
  return strncmp(command,test,strlen(test));
}

char* addChar(char* str, char c){
  int i;
  //Se a string for vazia cria uma com apenas um char
  if(str==NULL) {
    char* new = malloc(sizeof(char));
    new[0]=c;
    return new;
  }
  //Caso exista o char na string, retorna a mesma
  for(i=0; i < strlen(str); i++) {
    if(str[i] == c) return str;
  }
  //Cria uma string com mais um elemento que a str
  char *new = malloc(sizeof(char)*(i+1));
  for(i=0;i<strlen(str);i++){
    new[i]=str[i];
  }
  new[i]=c;
  return new;
}

char* removeChar(char* str, char c){
  int i;
  int size = 0;
  char* new = NULL;
  //Verifica existência do char aka número de ocorrências diferentes
  for(i=0;i<strlen(str);i++) {
    if(str[i] != c) {
      size++;
    }
  }
  //Vê se existe o char
  if(size==strlen(str)) {
    return str;
  }
  int j=0;
  //Se existe o char
  if(size!=0) {
    new = malloc(sizeof(char)*size);
    for(i=0;i<strlen(str);i++) {
      if(str[i] != c){
        new[j]=str[i];
        j++;
      }
    }
  }
  return new;
}

/*
int readC(char** commands) {
  char* buf = malloc(sizeof(char)*25);
  int* charRead = malloc(sizeof(int));
  int n;
  int i=0;
  while((n = readLn(0,buf,1,charRead)) != -1) {
    char* new = malloc(sizeof(char)*strlen(buf));
    strcpy(new,buf);
    commands[i] = new;
    i++;
  }
  return i;
}*/


int main() {
  char* buf= malloc(sizeof(char)*100);   //buffer para a leitura do input
  int* c = malloc(sizeof(int));  //
  int n;
  while((n = readLn(0,buf,1,c)) != -1) {
    char* aux = malloc(sizeof(char)*strlen(buf));
    strcpy(aux,buf);
    char* aux1 = malloc(sizeof(char)*strlen(buf));
    strcpy(aux1,buf);
    char* command = strtok(aux," ");
    char* _id = strtok(NULL," ");
    int id = atoi(_id);
    if(!(prefix(aux,"node"))) {
        char* componente = strtok(NULL," ");
        char* argumentos = strtok(NULL, "\n");
        printf("%d\n%s\n%s\n",id,&*componente,&*argumentos);

        //execvp(componente[0],argumentos)

    }
    else if (!(prefix(aux,"connect"))){
      int i = 0;
      char* campo;
      while((campo = strtok(NULL," ")) != NULL) {
        i++;
      }
      int ids[i];
      int j=0;
      char* _campo = strtok(aux1," ");
      _campo = strtok(NULL, " ");
      while((_campo = strtok(NULL," ")) != NULL) {
        char* aux2 = malloc(sizeof(char)*strlen(_campo));
        strcpy(aux2,_campo);
        ids[j]=atoi(aux2);
        j++;
      }
      for(i=0;i<j;i++){
        printf("o id %d vai se ligar ao id %d\n",id,ids[i]);
      }
    }
    else if (!(prefix(aux,"disconnect"))) {
      char* _id2 = strtok(NULL, "\n");
      int id2 = atoi(_id2);
      printf("O id %d vai se desconectar do id %d\n",id,id2);
      // Restante Código de disconnect
    }
	}
  return 0;
}
