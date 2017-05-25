#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

//incompleto
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

int constant(char *num){
	int len = strlen(num);
    int r = 1, i, n;
    char buf[1024];
    while(r){
        //n = sprintf(buf, "% 6d  ", i);
        r = readln(0, buf, sizeof(buf) - len);
        if(r == 0) break;
		buf[sizeof(buf)-len] = atoi(num);
        write(1, buf, r+len);
		write(1, num, len);
    }
    return 0;
}
//

int filter (char *col, char *op, char *oper){
return 1;
}

/*char **leitura (){
		int o, i j;
		o = open("input.txt", O_RDONLY);
		if (o > 0){
           for(i=0; i!=NULL;i++);
				for(j
}*/

int main(int argc, char **argv){
	
	
	//leitura();
	if(strcmp(argv[1],"const")==0) constant (argv[2]);
	if(strcmp(argv[1],"filter")==0) filter(argv[1], argv[2], argv[3]);
	else return 1;
	
	return 0;
}

