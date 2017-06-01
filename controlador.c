#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /*File control -> O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "readln.h"
#define PIPE_BUF 128



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


char* nome(int id) {
  char* pipeNome = malloc(sizeof(char)*id);
  while(id!=0){
    pipeNome=addChar(pipeNome,a);
  }
  return pipeNome;
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
  /*for(i=0; i < strlen(str); i++) {
    if(str[i] == c) return str;
  }*/
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


int run() {
  int i;
  int* nodosLigados[10];
  int nNodosLigados[10];
  for(i=0;i<10;i++){
    nodosLigados[i]=malloc(sizeof(int));
    nNodosLigados[i]=0;

  }

  char* buf= malloc(sizeof(char)*100);   //buffer para a leitura do input
  int* c = malloc(sizeof(int));  //
  int n;
  int i;
  int sair = 0;
  while((n = readLn(0,buf,1,c))) {
    if(!(prefix(buf,"sair"))) break;
    char* aux = malloc(sizeof(char)*strlen(buf));
    strcpy(aux,buf);
    char* aux1 = malloc(sizeof(char)*strlen(buf));
    strcpy(aux1,buf);
    char* command = strtok(aux," ");
    int id = atoi(strtok(NULL," "));
    if(!(prefix(aux,"node"))) {
        char* componente = strtok(NULL," ");
        /*Passamos com uma string só ou cada indice um argumento?*/
        char* argumentos = strtok(NULL, "\n");
        printf("%d\n%s\n%s\n",id,&*componente,&*argumentos);
        /*Retante codido de criação de pipe*/
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
        ids[j]=atoi(aux2); /*Armazena em ids[j] os vários id a connectar */
        j++;
      }
      for(i=0;i<j;i++){
        printf("o id %d vai se ligar ao id %d\n",id,ids[i]);
      }
    }
    else if (!(prefix(aux,"disconnect"))) {
      //char* _id2 = strtok(NULL, "\n");
      int id2 = atoi(strtok(NULL," ")); /*Id a desconectar*/
      if(id2==0 || id ==0 || (strtok(NULL," ") != NULL)) printf("Introduza uma linha válida!\n");
      else{
        printf("O id %d vai se desconectar do id %d\n",id,id2);
        char* disconnect = "disconnect";
        char _id = id;
        char _id2 = id2;
        char* mensagem = malloc(sizeof(char)*12);
        //Disconnect 'id a desconectar'
        for(i=0;i<10;i++) mensagem[i]=disconnect[i];
        mensagem[10] = ' ';
        mensagem[11] = codigo(_id2);
        mensagem[12] = '\n';
        printf("Mensagem a enviar: %s",&*mensagem);
        printf("teste: %c",mensagem[11]);

        //Procura o pipe
        char* str = "pipe_worker";
        char* nomePipe = malloc(sizeof(char)*12);
        for(i=0;i<10;i++) nomePipe[i]=str[i];
        nomePipe[10] = ' ';
        nomePipe[11] = codigo(_id);
        nomePipe[12] = '\n';

        int pipe = open(nomePipe, O_WRONLY);
        write(pipe,mensagem,12);
        close(pipe);
      }
      // Restante Código de disconnect
    }
    else printf("Introduza uma linha válida!\n");
	}
  printf("A sair do programa!\n");
  return 0;
}

int main (){
  run();
  return 1;
}
