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

void printbyte(char b){
	unsigned char c;
	for(c=128;c>0;c>>=1)
		printf("%d",(b&c)?1:0);
}

void printmac(u_char *d){
	for(int i=0;i<6;i++){
			printf("%02x", d[i]);
			if(i<5)
				printf(":");
		}
}
int loneth(u_char c[2],u_short *tam){
	/*printf("\nprimer byte ");
	printbyte(c[0]);
	printf("\nsegundo byte ");
	printbyte(c[1]);*/
	u_short total=0;
	total=c[0];
	total<<=8;
	total=total|c[1];
	// printf("\ntam: %hu",total);
	*tam=total;
	if(total<=1500)//**Compara para saber si el campo es menor o igual a 1500
		return 0;
	else
		return 1;
}


int examen(const u_char trama[]/*,int longitud, char resultado[],int longres*/){
	u_short ethertype=0;
	printf("Funcion examen\n");
	//cabecera ethernet
	u_char MAC_DEST[6];
	u_char MAC_SOURCE[6];
	u_char TYPE[2];
	printf("Trama:\n");
	printhex(trama);
	memcpy(MAC_DEST,&trama[0],6);
	memcpy(MAC_SOURCE,&trama[6],6);
	memcpy(TYPE,&trama[12],2);
	printf("\n\nMac destino: ");
	printmac(MAC_DEST);
	printf("\nMac origen: ");
	printmac(MAC_SOURCE);
	printf("\nTipo: %02x %02x\n", TYPE[0],TYPE[1]);
	//Aqui se ve si es ethertype o longitud
	// printf("\n y es: %s",loneth(TYPE)?"tipo":"longitud");
	if(loneth(TYPE,&ethertype)){
		printf("\nTrama ethertype");
		switch(ethertype){
			case 2034:
				printf("\nLa trama es de IP");
				break;
		}
	}
	else{
		printf("\nTrama LLC");
		u_char DSAP=trama[14],SSAP=trama[15];
		printf("\nDSAP: %02x",DSAP);
		printf("\nSSAP: %02x",SSAP);
		//Para REJ
		if((trama[16]&1))
			if(trama[16]==0x09)
				printf("\n LLC REJ");
	}
	return 0;
}


int main(){
	printf("**ETS Practico**\n\n");
	examen(pkt1);
	return 0;
}
