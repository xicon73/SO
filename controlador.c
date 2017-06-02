#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /*File control -> O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <limits.h>



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

/*Aplica o nome de "id*a"*/
char* nome(int id) {
    char* name = malloc(sizeof(char)*(id + 1));
    char a = 'a';
    int i = 0;
    while(i<id){
      name[i] = a;
      i++;
    }
    name[i] = '\0';
    return name;
}

/*Compara o buf com test, até este chegar ao fim*/
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

int* removeNodo(int* listaNodos, int* tamanho, int id) {
  int teste = 0;
  int i;
  int j = 0;
  int* aux = malloc(sizeof(int)*(*tamanho));
  for(i = 0; i<(*tamanho); i++) {
    if(listaNodos[i]==id) {
      teste=1;
    }
    else if((listaNodos[i]>id) && (teste!=1)) break;
    else{
      aux[j]=listaNodos[i];
      j++;
    }
  }
  if(teste==0) perror("Não existe ligação ao nodo inserido!\n");
  else {(*tamanho)--;return aux;}
  return listaNodos;
}

int* adicionaNodo(int* listaNodos, int* tamanho, int id) {
  int teste = 0, j = 0, i;
  int* aux = malloc(sizeof(int)*((*tamanho)+1));
  if((*tamanho)==0) {aux[0]=id; (*tamanho)++;}
  for(i=0; i<(*tamanho); i++) {
    if(listaNodos[i]==id) return listaNodos;
    if(listaNodos[i]<id) { aux[j]=listaNodos[i]; j++;}
    if((listaNodos[i]>id) && teste == 0) {
      aux[j]=id; j++;
      aux[j]=listaNodos[i]; j++;
      teste=1;
    }
    else {aux[j]=listaNodos[i]; j++;}
    }
    return aux;
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

void s(){
  _exit(0);
}

int run() {
  int* nodosLigados[10];
  int nNodosLigados[10];
  pid_t pid1[10];//Pipe que recebe os comandos e executa os componentes
  pid_t pid2[10];//Pipe que recebe o resultado dos componentes
  pid_t pid3[10];//Pipe que envia e connecta o que recebe de 2 para o/s nodo/s
  char* buf= malloc(sizeof(char)*100);   //buffer para a leitura do input
  int* c = malloc(sizeof(int));  //
  int n;
  int sair = 0;
  int i;

  for(i=0;i<10;i++){
    nodosLigados[i]=malloc(sizeof(int));
    nNodosLigados[i]=0;
    pid1[i]=-1;
    pid2[i]=-1;
    pid3[i]=-1;
  }


  while((n = readLn(0,buf,1,c))) {
    if(strlen(buf)<4) perror("Introduza uma linha válida\n");
    else{
      if(!(strncmp(buf,"sair",4))) break;
      char* aux = malloc(sizeof(char)*strlen(buf));
      strcpy(aux,buf);
      char* aux1 = malloc(sizeof(char)*strlen(buf));
      strcpy(aux1,buf);
      char* command = strtok(aux," ");
      int id = atoi(strtok(NULL," "));
      if(!(strncmp(aux,"node",4))) {
        char* _id = malloc(sizeof(char));
        sprintf(_id,"%d",id);
        char* componente = strtok(NULL," ");
        /*Passamos com uma string só ou cada indice um argumento?*/
        char* argumentos = strtok(NULL, "\n");
        printf("%d\n%s\n%s\n",id,&*componente,&*argumentos);
        /*Retante codido de criação de pipe*/
        //execvp(componente[0],argumentos)

        int pipe2[2];
        pipe(pipe2);

        pid_t pid_1 = fork();
        if(!pid_1) {

          signal(SIGUSR1,s);

          int pipe0;
          char* nomePipe0 = malloc(sizeof(char)*(strlen(_id)+5));
          nomePipe0 = strcat(nomePipe0,"pipe0");
          nomePipe0 = strcat(nomePipe0,_id);

          mkfifo(nomePipe0,0666);

          pipe0 = open(nomePipe0, O_RDWR);

          dup2(pipe0,0);
          close(pipe0);

          close(pipe2[0]);
          dup2(pipe2[1],1);
          close(pipe2[1]);

          //execvp(componente[0],argumentos)
        }

        pid1[id] = pid_1;

        char* nomePipe3 = malloc(sizeof(char));
        nomePipe3 = strcat(nomePipe3,"pipe3");
        nomePipe3 = strcat(nomePipe3,_id);

        mkfifo(nomePipe3,0666);

        pid_t pid_2 = fork();
        if(!pid_2){
          signal(SIGUSR1,s);
          int pipe3;
          pipe3 = open(nomePipe3, O_WRONLY);

          close(pipe2[1]);

          char* output = malloc(sizeof(char)*PIPE_BUF);
          int* nCarateresL = malloc(sizeof(int));
          int o;


          while((o = readLn(pipe2[0],output,1,nCarateresL))) {
            write(pipe3,output,*nCarateresL);
          }

          close(pipe2[0]);
          close(pipe3);
          _exit(0);
        }

        pid2[id] = pid_2;

        pid_t pid_3 = fork();
      if(!pid_3) {
        signal(SIGUSR1,s);
        close(pipe2[0]);
        close(pipe2[1]);

        int* ndos = NULL;
        int numero = 0;
        int pipe3;

        pipe3 = open(nomePipe3, O_RDWR);

        int o;
        char* output = malloc(sizeof(char)*PIPE_BUF);
        int* nCarateresL = malloc(sizeof(int));

        while((o = readLn(pipe3,output,1,nCarateresL))) {
          if(strncmp("connect",output,7)){
            char * temp = strtok(output," ");
            while((temp = strtok(NULL," ")) != NULL) {
              int idNovo = atoi(temp);
              ndos = adicionaNodo(ndos,&numero,idNovo);
              break;
            }
          }
          else if(strncmp("disconnect",output,10)){
            char * temp = strtok(output," ");
            while((temp = strtok(NULL," ")) != NULL) {
              int idVelho = atoi(temp);
              ndos = removeNodo(ndos,&numero,idVelho);
              break;
            }
          }
          else {
            for(int k = 0; k < numero; k++) {
              char* nomePipe0 = malloc(sizeof(char));
              sprintf(_id,"%d",ndos[k]);
              nomePipe0 = strcat(nomePipe0,"pipe0");
              nomePipe0 = strcat(nomePipe0,_id);

              int pipe0 = open(nomePipe0,O_WRONLY);
              write(pipe0,output,*nCarateresL);

              close(pipe0);
            }
          }
        }
        close(pipe3);
        _exit(0);
      }
      pid3[id]=pid_3;
    }
    else if (!(strncmp(aux,"connect",7))){
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
        nodosLigados[id]=adicionaNodo(nodosLigados[id],&nNodosLigados[id],ids[j]);
        j++;
        printf("Está lido a este nodo: %d\n",*nodosLigados[id]);
      }
      for(i=0;i<j;i++){
        printf("o id %d vai se ligar ao id %d\n",id,ids[i]);
      }
      char* _id = malloc(sizeof(char));
      sprintf(_id,"%d",id);
      char* _id2 = malloc(sizeof(char));
      char* mensagem = malloc(sizeof(char));
      mensagem = strcat(mensagem,"connect ");
      for(i=0;i<j;i++){
        sprintf(_id2,"%d",ids[i]);
        mensagem = strcat(mensagem,_id2);
        if(i!=j-1) mensagem = strcat(mensagem, " ");
      }
      mensagem = strcat(mensagem,"\n");

      printf("Mensagem a enviar: %s",&*mensagem);

      char* nomePipe3 = malloc(sizeof(char));
      nomePipe3 = strcat(nomePipe3,"pipe3");
      nomePipe3 = strcat(nomePipe3,_id);
      printf("nome do pipe: %s",&*nomePipe3);
      int pipe3;

      pipe3 = open(nomePipe3, O_WRONLY);
      write(pipe3,mensagem,strlen(mensagem));

      close(pipe3);
      }
      else if (!(strncmp(aux,"disconnect",10))) {
      //char* _id2 = strtok(NULL, "\n");
      int id2 = atoi(strtok(NULL," ")); /*Id a desconectar*/
      if(id2==0 || id ==0 || (strtok(NULL," ") != NULL)) perror("Introduza uma linha válida!\n");
      else{
        int pipe3; //pipe que envia a linha do nodo id
        //printf("O id %d vai se desconectar do id %d\n",id,id2);
        int teste = nNodosLigados[id];
        //nodosLigados[id] = removeNodo(nodosLigados[id],&nNodosLigados[id],id2);
        if(teste==nNodosLigados[id]) {
          char* _id = malloc(sizeof(char));
          sprintf(_id,"%d",id);
          //printf("%c\n",id);
          char* _id2 = malloc(sizeof(char));
          sprintf(_id2,"%d\n",id2);


          char* mensagem = malloc(sizeof(char));

          //Disconnect 'id a desconectar'
          mensagem = strcat(mensagem,"disconnect ");
          mensagem = strcat(mensagem, _id2);
          printf("Mensagem a enviar: %s",&*mensagem);

          char* nomePipe3 = malloc(sizeof(char));
          nomePipe3 = strcat(nomePipe3,"pipe3");
          nomePipe3 = strcat(nomePipe3,_id);
          printf("nome do pipe: %s",&*nomePipe3);

          pipe3 = open(nomePipe3, O_WRONLY);
          write(pipe3,mensagem,strlen(mensagem));

          close(pipe3);
        }
      }
      // Restante Código de disconnect
    }
      else if (!(strncmp(aux,"inject",6))) {
      }
      else perror("Introduza uma linha válida!\n");
    }
	}
  printf("A sair do programa!\n");
  return 0;
}

int main (){
  run();
  return 1;
}
