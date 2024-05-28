#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 * Autores: David Lozano Bañuelos
 *          Bernardo Nava Trinidad
 *          Sergio Andres Guerrero Reyes
*/

//declaracion de funciones a usar en todo el codigo
void error(int tipo, char **manejo);

void main(int argc, char *argv[]){
    FILE * archivo_entrada;
    FILE * archivo_salida;
    //verificamos si se esta pasando el archivo a analizar
    if(argc!=2){
        error(00,argv);
    }else{
        //intentamos abrir el archivo, el cual tiene que estar creado
        //r — abre el archivo en modo de solo lectura. 
        archivo_entrada=fopen(argv[1],"r");
        //a — abre el archivo para agregar información (si no existe lo crea).
        archivo_salida=fopen("compilado.asm","a");
        //si no se puede abrir dara error
        if(!archivo_entrada){
            error(01,argv);        
            if(!archivo_salida){
                error(02,argv);
            }
        //si se pudo leer el archivo de entrada y se crea el archivo de salida, podemos continuar
        }else{

        }
    }
}

//funcion para el manejo de errores
void error(int tipo, char **manejo){
        switch (tipo){
            case 00:
                printf("\nError 0: cantidad de argumentos invalidos o archivo no encontrado.\n");
                printf("----> \"%s\" no encontrado, favor de verificar.",manejo[1]);
                exit(0);
            break;
            case 01:
                printf("\nError 1: Archivo no encontrado o mal escrito.\n");
                printf("----> \"%s\" no se encontro o esta mal escrito.\nRecuerda que tambien se pone la extension del archivo.",manejo[1]);
                exit(1);
            break;
            case 02:
                printf("\nError 2: No se pudo acceder o crear el archivo solicitado.\n");
                exit(2);
            break;
        }
}
