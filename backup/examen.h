/* 
 * io.h
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * System level I/O functions and types.
 *
 */
#ifndef	_EXAMEN_H_
#define	_EXAMEN_H_
void registro(char *);
void transmitir(char ip[],char mensaje[]);
//int examen(unsigned char trama[],int longitud, char resultado[],int longres);
int establecefiltro(unsigned char npatron[],int ntamano, int ndesplazamiento);
//void filtrar(void);

#endif	/* _IO_H_ not defined */
