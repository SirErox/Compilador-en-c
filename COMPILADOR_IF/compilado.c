#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/*
 * Autores: David Lozano Bañuelos
 *          Bernardo Nava Trinidad
 *          Sergio Andres Guerrero Reyes
*/

//declaracion de funciones a usar en todo el codigo
void error(int tipo, char **manejo);
void cerrarasm(FILE * manejo);

void main(int argc, char *argv[]){
    FILE * archivo_entrada;
    FILE * archivo_salida;
    //verificamos si se esta pasando el archivo a analizar
    if(argc!=2){
        error(00,argv);
//de ser asi, checamos si es un .c, ya que es el tipo de archivo que necesitamos
    } else if( (strstr(argv[1],".c"))==NULL){
        //si no se encuentra, dara error 1, indicando que se debe revisar el nombre pasado como argumento
        error(01,argv);
    }else{
        //intentamos abrir el archivo, el cual tiene que estar creado
        //r — abre el archivo en modo de solo lectura. 
        archivo_entrada=fopen(argv[1],"r");
        //a — abre el archivo para agregar información (si no existe lo crea).
        archivo_salida=fopen("compilado.asm","a");
        if(!archivo_entrada)
            error(01,argv);        
            if(!archivo_salida){
                error(02,argv);
            
        //si se pudo leer el archivo de entrada y se crea el archivo de salida, podemos continuar
        }else{
            printf("\nArchivo encontrado, abriendo...");
            cerrarasm(archivo_salida);
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
                printf("----> \"%s\" no se encontro o esta mal escrito.\n",manejo[1]);
                printf("Te recuerdo que el programa necesita un archivo con extension .c, otro tipo dara error.");
                exit(1);
            break;
            case 02:
                printf("\nError 2: No se pudo acceder o crear el archivo solicitado.\n");
                exit(2);
            break;
        }
}

void cerrarasm(FILE * manejo){
//se agregan hasta el final del archivo las lineas 
//mov eax,1
//int 80h
fseek(manejo,0,SEEK_END);
fwrite("mov eax,1\n",1,strlen("mov eax,1\n"), manejo);
fwrite("moc ebx,0\n",1,strlen("mov ebx,0\n"), manejo);
fwrite("int 80h\n",1,strlen("int 80h\n"), manejo);
}

void compiladoasm(FILE * asm){
    
}