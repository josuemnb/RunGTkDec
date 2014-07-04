#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Error.h"

int erro=0;

int showErr(int msg){
	char s[80];

	switch(msg){
		case 1:strcpy(s,"Falta memoria");break;
		case 2:strcpy(s,"Ficheiro não existe");break;
		case 3:strcpy(s,"Ficheiro corrompido");break;
		case 4:strcpy(s,"Versao do Codec do ficheiro é superior a instalada");break;
		case 5:strcpy(s,"Ficheiro incompativel");break;
        case 6:strcpy(s,"Inesperado Fim da linha");break;
        case 7:strcpy(s,"Inesperado Fim do ficheiro");break;
		default:strcpy(s,"Erro desconhecido");break;
	}
	erro++;
    printf("\nERROR: %s\n",s);
	//printf("\n\tErro na linha %d - %s\n\t\t%s - %s\n",LineIndex,Line,s,token);
	exit(msg);
}


int get_errors(){
    return erro;
}
