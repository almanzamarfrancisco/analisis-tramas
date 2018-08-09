//Practica ETS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprimirchar(char b){
	unsigned char c;
	for(c=128;c>0;c>>=1)
		printf("%d",(b&c)?1:0);
}
void imprimiearrhex(u_char *cad,int length){
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
	tam=tl[0];
	tam<<=8;
	tam=tam|tl[1];
	return tam;
}

int llc( u_char *t, int size, u_char *res ){
	u_char *s = malloc( sizeof(u_char)*12 ), control=0,controls=0;
	u_short ext = 1//Extendido?;
	memcpy( s, &t[0], 6 );
	printf("\nMac Destino: ");imprimiearrhex( s, 6 );
	memcpy( s, &t[6], 6 );
	printf("\nMac Fuente: ");imprimiearrhex( s, 6 );
	memcpy( s, &t[12], 6 );
	printf("\nTipo o tamaño: %i", analizarTL( s ));
	if( analizarTL( s ) <= 1500 ){
		printf("\n**Trama LLC**");
		control = t[16];
		controls = t[17];
		printf("\n%02x %02x", control, controls);
		if((control&3)==3){
				puts("\nNo Numerada");
				switch(control){
				case 0x03:
					memcpy(res,"UI  ",4);
					break;
				case 0x43:
				case 0x53:
					memcpy(res,"DISC",4);
					break;
				case 0x63:
				case 0x73:
					memcpy(res,"UA",2);
					break;
				case 0x6f:
				case 0x7f:
					ext=1;
					memcpy(res,"SABME",5);
					break;
				default:
					memcpy(res,"UNKN",4);
				}
			}else
			if(control&1==1){
				printf("\nSupervision ");
				switch(control){
					case 0x01:
					memcpy(res,"RR",2);
					break;
					case 0x05:
						memcpy(res,"RNR",3);
						break;
					case 0x09:
						memcpy(res,"REJ",3);
						break;
					default:
						memcpy(res,"   ",3);
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
				memcpy(res,"   ",3);
				printf("\nInformacion ");
				if(ext){
					printf("extendido");
					printf("\nN(S): %d",(control&254)>>1);
					printf("\nP/F: %d",(controls&1));
					printf("\nN(R): %d",(controls&254)>>1);
				}
				else
					puts("estandar");
			}
			printf("\ncontrol: %s", res);
			memcpy(res,"          ",10);
		
		// printf("\n%s\n", res);
		return 1;
	}
	return 0;
}

int main(){
	u_char trama[60] = {
		0x00, 0xa0, 0x24, 0x14, 0x49, 0x4a, 0x00, 0xa0,
		0x24, 0x14, 0x49, 0xaa, 0x00, 0x04, 0xf0, 0xf0,
		0x0a, 0x3b, 0x00, 0x00, 0xff, 0xef, 0x16, 0x0c,
		0x00, 0x00, 0x28, 0x00, 0x28, 0x00, 0x0c, 0x0f,
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x20, 0x20, 0x20, 0x20                         
	};
	u_char resultado[100]/* = "soy resultado"*/;
	if(llc(trama,sizeof(trama),resultado))
		printf("\n\nEl resultado es:\n %s",resultado);
	return 0;
}