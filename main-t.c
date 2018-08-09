//Analzador LLC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprimirchar(char b){
	unsigned char c;
	for(c=128;c>0;c>>=1)
		printf("%d",(b&c)?1:0);
}
void imprimiearrhex(u_char *cad,int length){//aqui hya una manera para tener el length de un arreglo
	for(int i=0,j=0;i<length;i++,j++){
		printf("%02x ",cad[i]);
		if(j==15){
			putchar('\n');
			j=0;
		}
	}
}
u_short analizarTL(u_char* tl){
	u_short tam;
	//imprimirchar(tl[0]);
	//imprimirchar(tl[1]);
	tam=tl[0];
	tam<<=8;
	tam=tam|tl[1];
	//printf("\ntam: %hu",tam);
	return tam;
}
int aceptable(char c){
	if((c>=97&&c<=103)||(c>=48&&c<=57))
		return 1;
	return 0;
}

void main(){
	int i=0, j=0,t=0;
	u_char eth[31],MACOR[6],MACDEST[6],TL[2],DSAP=0,SSAP=0,control=0,controls=0;
	char ig[2][11]={"individual","en grupo"},cr[2][10]={"comando","respuesta"};
	char rescontrol[10];
	u_short ext=0;
	FILE *f=fopen("tramas LLC.txt", "r");
	puts("\n**ANALIZADOR DE LLC**");
	if(f==NULL){
	  perror("Error, no se encontró archivo");
	  return;
   }
	//Hacer bufer para el texto
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  //same as rewind(f);
	char *string = malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);
	string[fsize] = 0;//printf("%s",string);
	char hex[3]="  ";hex[2]='\0';
	//toma los valores en hexadecimal //hay que hacer valores bidimensional para que tenga tramas
	for(i=0;string[i];i++)//Para ponerle el tamaño a valores se contaron los espacios
		if(string[i]==' ')
			j++;//se contó con j
	u_char valores[j+1];//printf("j: %d",j);
	for(i=0,j=0;i<=sizeof(valores)/sizeof(u_char);i++){
		if(string[i]==' '){
			memcpy(hex,&string[i-2],2);
			if(aceptable(hex[0])&&aceptable(hex[1])){//por alguna razón agregaba un cero que no iba
				//printf("%s ",hex);
				valores[j] = (int)strtol(hex, NULL, 16);//Toma los valores en hexadecial y añade el valor hexadecimal arreglo de valores hedecimales
				//printf("%02x ",valores[j]);
				j++;
			}
		}
	}
	//imprimiearrhex(valores,sizeof(valores)/sizeof(u_char));//para imprimir todo valores 
	 //printf("%02x ",valores[i]);
	i=0;
	do{
		memcpy(eth,&valores[i],30);
		puts("\n*ETHERNET*");
		printf("eth: ");
		imprimiearrhex(eth,30);
		memcpy(MACOR,&eth[0],6);
		memcpy(MACDEST,&eth[6],6);
		printf("\nMAC Origen : ");
		imprimiearrhex(MACOR,6);
		printf("\nMAC Destino: ");
		imprimiearrhex(MACDEST,6);
		memcpy(TL,&valores[i+12],2);
		t=analizarTL(TL);
		printf("\nTipo o Longitud: %d",t);
		if(t<1500){
			printf("\n*LLC*");
			DSAP=valores[i+14];
			SSAP=valores[i+15];
			printf("\nDSAP %s: ",ig[DSAP&1]);imprimirchar(DSAP);
			printf("\nSSAP %s: ",cr[SSAP&1]);imprimirchar(SSAP);
			control=valores[i+16];
			controls=valores[i+17];
			if((control&3)==3){
				puts("\nNo Numerada");
				switch(control){
				case 0x03:
					memcpy(rescontrol,"UI  ",4);
					break;
				case 0x07:
				case 0x17:
					memcpy(rescontrol,"SIM ",4);
					break;
				case 0x2f:
				case 0x3f:
					memcpy(rescontrol,"SABM",4);
					break;
				case 0x43:
				case 0x53:
					memcpy(rescontrol,"DISC",4);
					break;
				case 0x63:
				case 0x73:
					memcpy(rescontrol,"UA",2);
					break;
				case 0x6f:
				case 0x7f:
					ext=1;
					memcpy(rescontrol,"SABME",5);
					break;
				case 0x83:
				case 0x93:
					memcpy(rescontrol,"SNRM",4);
					break;
				case 0x87:
				case 0x97:
					memcpy(rescontrol,"FRMR",4);
					break;
				case 0x8f:
				case 0x9f:
					memcpy(rescontrol,"SNRME",4);
					break;
				case 0xaf:
				case 0xbf:
					memcpy(rescontrol,"XID",3);
					break;
				case 0x0f:
				case 0x1f:
					memcpy(rescontrol,"SARM",4);
					break;
				case 0xe3:
				case 0xf3:
					memcpy(rescontrol,"TEST",5);
					break;
				default:
					memcpy(rescontrol,"UNKN",4);
				}
			}else
			if(control&1==1){
				printf("\nSupervision ");
				switch(control){
					case 0x01:
					memcpy(rescontrol,"RR",2);
					break;
					case 0x05:
						memcpy(rescontrol,"RNR",3);
						break;
					case 0x09:
						memcpy(rescontrol,"REJ",3);
						break;
					default:
						memcpy(rescontrol,"   ",3);
				}
				if(ext){
					printf("extendido");
					printf("\nP/F: %d",(controls&1));
					printf("\nN(R): %d",(controls&254)>>1);
				}
				else{
					puts("estandar");
				}
			}else
			if(!(control&0)){
				memcpy(rescontrol,"   ",3);
				printf("\nInformacion ");
				if(ext){
					printf("extendido");
					printf("\nN(S): %d",(control&254)>>1);
					printf("\nP/F: %d",(controls&1));
					printf("\nN(R): %d",(controls&254)>>1);
				}
				else{
					puts("estandar");
				}
			}
			printf("\ncontrol: %s", rescontrol);
			memcpy(rescontrol,"          ",10);
		}else
			printf("\n*No es LLC*");
		printf("\nno:%d\ti:%d",i/60,i);
		if(t<60)
			i+=60;
		else
			i+=t+14;
		//printf("\naumentado en: %d",i);
		getchar();
	}while(i<sizeof(valores)/sizeof(u_char));
}




//http://148.204.58.221/ealdana/redesnp/Diapositivas/
