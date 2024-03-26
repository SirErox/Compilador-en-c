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
//aqui le decimos al programa cuanto es el maximo permitido de palabras a encontrar
#define MAX_PALABRA_RESERVADA 500
//declaracion de funciones a usar en el programa
void buscar_palabra_Reserv(FILE * archivo, int num);
void imprimir_reservadas();
//arreglo y variables para las palabras reservadas encontradas
char *reservadas_encontradas[MAX_PALABRA_RESERVADA];
int num_palabras_reservadas=0;
//arreglo de palabras reservadas a comparar
const char *palabra_reservada[]={
    // --> "", para agregar palabras reservadas, se pone la palabra tal y como aparece en medio de las comillas
    "if",
    "else",
    "for",
    "int",
    "float",
    "double",
    "main",
    "return",
    "do",
    "while",
    "char",
    "NULL",
    "switch",
    "case",
    "break",
    "printf",
    "const",
    "define",
    "include",
    "void",
    "argc",
    "argv",
    NULL
};

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
            //opcion 1 palabras reservadas
                buscar_palabra_Reserv(archivo,num);
                fclose(archivo);
                imprimir_reservadas();
                break;
            case 2:
            //opcion 2 identificadores aA-zZ
                printf("caso 2");
                fclose(archivo);
                break;
            case 3:
            //opcion 3 numeros 0 1 2 3 4 5 6 7 8 9
                printf("caso 3");
                fclose(archivo);
                break;
            case 4:
            //opcion 4 operadores + - / * < > =
                printf("caso 4");
                fclose(archivo);
                break;
            case 5:
            //opcion 5 comentarios /* */ //
                printf("caso 5");
                fclose(archivo);
                break;
            default:
                fclose(archivo);
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

void buscar_palabra_Reserv(FILE * archivo, int num){
   char palabra[20]; // La longitud de la palabra debería ser suficiente para tu caso de uso
    int caracter;
    int index = 0;
    //loop principal, leeremos los caractere hasta encontrar EOF
    while ((caracter = fgetc(archivo)) != EOF) {
        //preguntamos si es una letra con "isaplha" o si es un -
        //si es una letra, la convertimos a caracter y lo pasamos al arreglo de palabra
        if (isalpha(caracter) || caracter == '_') {
            palabra[index++] = (char) caracter;
            palabra[index] = '\0';
        } else {
            //si i es mayor a cero, quiere decir que tenemos algo a procesar
            if (index > 0) {
                index = 0;
                //por lo tanto tenemos que comparar esa palabra con el arreglo predefinido
                bool encontro = false;
                //recorremos todo el arreglo predefinido hasta el null
                for (const char **word = palabra_reservada; *word != NULL; word++) {
                    //comparamos las cadenas y si coinciden, encontro se vuelve true
                    if (strcmp(*word, palabra) == 0) {
                        encontro = true;
                        break;
                    }
                }
                //como si se encontro una palabra coincidente, la guardamos en el arreglo
                if (encontro) {
                    //strdup crea una copia de la palabra y devuelve un puntero a ella
                    reservadas_encontradas[num_palabras_reservadas++] = strdup(palabra);
                }
            }
        }
    }
}
//aqui solo imprimimos en pantalla las palabras reservadas encontradas
//indicamos el total encontrado y cuales fueron
void imprimir_reservadas(){
    printf("Total de palabras reservadas encontradas:%i\n",num_palabras_reservadas);
    for(int i=0;i<num_palabras_reservadas;i++){
        printf("%i : %s\n",i+1,reservadas_encontradas[i]);
    }
}