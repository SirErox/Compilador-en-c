#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
//ubicacion del puntero, para el manejo de errores
int linea=1,columna=1;
//columna es la posicion en la linea
// y la linea es cada linea del documento

//declaracion de funciones para usarse en el documento
void errores(int linea, int columna, char caracter);

int main(int argc, char *argv[]){
    FILE * archivo;
    char caracter;
    char expresion[25];
    int i;
    archivo=fopen(argv[1],"r");
    if(!archivo){ //<-- si el archivo no existe dara error
        printf("archivo no existente:%s",argv[1]);
        return 1;
    }else{
        //el archivo existe, por lo tanto leeremos lo que contiene
       while((caracter=fgetc(archivo))!=EOF){
        printf("%c",caracter);
       }
    }
    return 0;
}


void errores(int linea, int columna,char caracter){
    printf("\nError en la pos: (%d,%d)\n",linea,columna);
}
