/*
La funcion examen es llamada cada que se recibe una trama; debe analizar la trama y en caso
de cumplir con los casos definidos en el enunciado del examen llenar la 
cadena resultado con los datos solicitados y regresar un valor de 1. Si no concuerda con 
ninguna de las tramas solicitadas debe regresar 0;

El programa requiere que se introduzca la IP que le proporcione el proferos como argumento, 
por ejemplo, si se ejecuta desde una ventana de sistema:
            
            examen.exe 148.204.58.221
            
o si se ejecuta desde el IDE de Dev C++, introducirla mediante el menú:
      Execute -> Parameters... -> Parameters to pass to your programs
            
El inciso siguiente es un ejemplo de pregunta de examen, el proferor le indicará lo que debe
realizar por otro medio 

1.- un datagrama de IP que se encuentre fragmentado   --  devolver la IP origen 
    (en formato decimal separado por puntos)

*/


#include "examen.h"
#include "tramas.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printhex(const u_char *u){
	// printf("\nFuncion printhex \n");
	// printf("%lu\n", sizeof(u)*sizeof(char*));	
	int tamano=sizeof(u)*sizeof(char*);
	int salto=0;
	for(int i=0;i<tamano;i++){
		printf("%02x ", u[i]);
		salto++;
		if(salto==16){
			printf("\n");
			salto=0;
		}
	}
	printf("\n");
}

void printmac(char *d){
	for(int i=0;i<6;i++){
			printf("%02x", d[i]);
			if(i<5)
				printf(":");
		}
}

void ethernet_header(const u_char *t){
	//cabecera ethernet
	u_char MAC_DEST[6];
	u_char MAC_SOURCE[6];
	u_char TYPE[2];
	printf("Trama:\n");
	printhex(t);
	memcpy(MAC_DEST,&t[0],6);
	memcpy(MAC_SOURCE,&t[6],6);
	printf("\n\nMac destino: ");
	printmac(MAC_DEST);
	printf("\nMac origen: ");
	printmac(MAC_SOURCE);
	printf("\nTipo: %02x %02x \n");
}

int examen(const u_char trama[]/*,int longitud, char resultado[],int longres*/){
	printf("Funcion examen\n");
	ethernet_header(trama);	
	return 0;
}


int main(){
	printf("**ETS Practico**\n\n");
	examen(pkt1);
	return 0;
}
