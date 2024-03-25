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
void Funcion1(FILE * nombre, int num);

int main(int argc,char *argv[]){
    FILE * archivo;
    char caracter;
    int num;
    //Validamos la cantidad de argumentos pasados al compilar, si no dara error
    if(argc!=3){
        printf("Error(0):Argumentos insuficientes\n\t Se requieren 2 argumentos");
        return 1;
    }else{
    // se obtiene el nombre del programa a comprobar, el argumento r es para abrir el archivo en modo lectura pero solo si el archivo existe
    archivo=fopen(argv[1],"r");
    if(archivo!=NULL){
        //Validamos el numero pasado como argumento
        if(sscanf(argv[2],"/%d",&num)==1 && argv[2]!=NULL){
            //Aqui se continua con la ejecucion del programa, ya que se comprobaron los argumentos y el numero es valido
            switch (num){
            case 1:
               Funcion1(archivo,num);
                break;
            case 2:
            printf("caso 2");
                break;
            case 3:
            printf("caso 3");
                break;
            case 4:
            printf("caso 4");
                break;
            case 5:
            printf("caso 5");
                break;
            default:
                printf("Error(3):El numero '%d' debe estar entre el 1 y el 5.\n\t Favor de checar el numero ingresado.",num);
                break;
            }
        } else{       
            printf("Error(2):Formato de argumento invalido.\n\t Debe ser '/<numero>'");
            return 1;
        }
    }
    else{
        printf("Error(1):Archivo '%s' no encontrado\n\t Favor de verificar el nombre del archivo, tambien se debe incluir la extension.",argv[1]);
        return 1; //regresamos 1 cuando se encuentra un error al abrir el archivo
    }
    }
    return 0;
}

void Funcion1(FILE * nombre, int num){
    char caracter;
    int i=-1;
    printf("Funcion 1 escogida.\nargumento:%d\n",num);
   caracter=fgetc(nombre);
   printf("%c",caracter);
   for(i=-1;caracter!=EOF;i++){
    caracter=fgetc(nombre);
    printf("%c",caracter);
   }
}