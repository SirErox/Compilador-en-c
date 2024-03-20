/** 
 * Etapa 1: lexico
 * Descripcion del programa: 
 * Autores: David Lozano Bañuelos
 *          Bernardo Nava Trinidad
 *          Andres...
*/
//declaracion de librerias a usar
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

int main(int argc,char *argv[]){
    FILE * archivo;
    char caracter;
    int num;
    
    // se obtiene el nombre del programa a comprobar
    archivo=fopen(argv[1],"r");
    if(archivo!=NULL){
        printf("Abriendo archivo\n");
    }
    else{
        printf("Error(1): Archivo no encontrado");
        return 1;
    }

    if(sscanf(argv[2],"/%d",&num)==1)
    //checamos si el argumento pasado es un numero valido o no
        printf("numero pasado:%d\n",num);
    else
    //de no estar bien ingresado el numero dara error
        printf("Formato de argumento invalido. \nDebe ser '/<numero>'");
    return 0;
}