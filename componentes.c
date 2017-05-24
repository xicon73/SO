#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>


//Função readln, feita no guião 1 exercício 6
int readln(int fildes, void *buf, size_t nbyte){
    int r = 1, tam = 0;
    char *c = buf;
    while(r){
        if(tam >= nbyte) return nbyte;
        r = read(fildes, c, 1);
        if (r == 0) break;
        if (r == -1) return -1;
        tam++;
        if(*c == '\n') break;
        c++;
    }
    return tam;
}

int constant (char *n){
	//char c ;
    int r = 1, en;
	char buf[1024];
    for(int i=1; r ; i++){
        r = readln (0, buf+i, sizeof(buf)-i);
        if (r == 0) break; 
	    write(1, buf, r+i);
   	   // write(1, n, 2);
    }
	
	//write(1, n , 2);
	return 1;
}




int main(int argc, char **argv){
	

	if(strcmp(argv[1],"const")==0){
			 constant (argv[2]);
	}
	else return 1;
	
	return 0;
}

