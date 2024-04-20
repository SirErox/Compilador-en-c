/** 
 * Etapa 1: lexico
 * Descripcion del programa: 
 * Autores: David Lozano Bañuelos
 *          Bernardo Nava Trinidad
 *          Sergio Andres Guerrero Reyes
*/
//declaracion de librerias a usar
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

//le decimos al programa cuanto es el maximo permitido de palabras a encontrar
#define MAX_PALABRA_RESERVADA 500

//le decimos al programa cuanto es el maximo permitido de numeros a encontrar
#define MAX_NUMEROS 500

#define MAX_OPERADORES 100

#define MAX_IDENTIFICADORES 1000

//declaracion de funciones a usar en el programa
void buscar_palabra_Reserv(FILE * archivo);
void imprimir_reservadas();

void busca_identificadores(FILE * archivo);
void imprimir_identificadores();

void busca_numeros(FILE * archivo);
void imprimir_numeros();

void busca_operadores(FILE * archivo);
void imprimir_operadores();

void busca_comentarios(FILE * archivo);

//arreglo y variables para las palabras reservadas encontradas
char *reservadas_encontradas[MAX_PALABRA_RESERVADA];
int num_palabras_reservadas = 0;

//arreglo y variables para los numeros encontrados
int numeros_encontrados[MAX_NUMEROS];
int num_numeros = 0;

char operadores_encontrados[MAX_OPERADORES];
int num_operadores = 0;

char *identificadores_encontrados[MAX_IDENTIFICADORES];
int num_identificadores  =0;

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
	"FILE",
	"default",
	"stdio.h",
	"stdlib.h",
	"stdbool.h",
	"ctype.h",
	"string.h",
	"math.h",
	"time.h",
	"short",
	"unsigned",
	"break",
	"continue",
	"enum",
	"goto",
	"sizeof",
	"static",	
    NULL
};

int main(int argc, char *argv[]){
    FILE * archivo;
    //char caracter;
    int num;
    //Validamos la cantidad de argumentos pasados al compilar, si no dara error
    if(argc!=3){
        printf("Error(0):Argumentos insuficientes\n\t Se requieren 2 argumentos");
        return 1;
    }else{
		// se obtiene el nombre del programa a comprobar, el argumento r es para abrir el archivo en modo lectura pero solo si el archivo existe
		archivo = fopen(argv[1], "r");
		if(archivo!=NULL){
			//Validamos el numero pasado como argumento
			archivo=fopen(argv[1], "r");
			if(sscanf(argv[2],"/%d", &num) == 1 && argv[2] != NULL){
				//Aqui se continua con la ejecucion del programa, ya que se comprobaron los argumentos y el numero es valido
				switch (num){
				case 1:
					//opcion 1 palabras reservadas
					buscar_palabra_Reserv(archivo);
					fclose(archivo);
					imprimir_reservadas();
					break;
				case 2:
					//opcion 2 identificadores aA-zZ
					busca_identificadores(archivo);
					fclose(archivo);
					imprimir_identificadores();
					break;
				case 3:
					//opcion 3 numeros 0 1 2 3 4 5 6 7 8 9
					busca_numeros(archivo);
					fclose(archivo);
					imprimir_numeros();
					break;
				case 4:
					//opcion 4 operadores + - / * < > =
					busca_operadores(archivo);
					fclose(archivo);
					imprimir_operadores();
					break;
				case 5:
					//opcion 5 comentarios /* */ //
					busca_comentarios(archivo);
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
	
//**********************************FUNCIONES DE BUSQUEDA DE ELEMENTOS DEL LEXICO**********************************	

//Funcion que encuentra las palabras reservadas y las guarda para imprimierlas posteriormente
void buscar_palabra_Reserv(FILE * archivo){
	char palabra[20]; // La longitud de la palabra deber�a ser suficiente para el caso de uso
    char caracter;
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


//Funcion que encuentra los identificadores (Aa - Zz) y los guarda para imprimierlos posteriormente
void busca_identificadores(FILE * archivo){
 char identificador[20];
    char caracter;
    int index = 0;
    int es_identificador = 0;

   while ((caracter = fgetc(archivo))!= EOF) {
        // Si es una letra o un caracter de subrayado, se agrega al identificador
        if (isalpha(caracter) || caracter == '_') {
            identificador[index++] = (char) caracter;
            identificador[index] = '\0';
            es_identificador = 1;
        } else if (caracter == '<') {
            // Si es un caracter <, se salta el resto de la cadena
            while ((caracter = fgetc(archivo))!= '>') {
                // No hacer nada por el momento
            }
		} else if (caracter == '\"' || caracter == '\'') {
            // Si es una comilla doble o simple, se salta el resto de la cadena
            while ((caracter = fgetc(archivo))!= '\"' && caracter!= '\'') {
                // No hacer nada
            }
        } else if (caracter == '/' && (caracter = fgetc(archivo)) == '/') {
            // Si es un comentario de línea, se salta el resto de la línea
			es_identificador=0; //no es identificador
            while ((caracter = fgetc(archivo))!= '\n') {
                // No hacer nada
            }
            // Se reinicia el identificador
            index = 0;
            es_identificador = 0;
        }else if (caracter == '/' && (caracter = fgetc(archivo)) == '*') {
            // Si es un comentario de bloque, se salta el resto de la cadena
				es_identificador=0; //no es un identificador
				while ((caracter = fgetc(archivo))!= '*' && (caracter=fgetc(archivo)!='\n')) {
					while( (caracter = fgetc(archivo))!= '/'){
						//no hacer nada
					}
					//no hacer nada
            	}
				index=0;
				es_identificador=0;
        } else {
            // Si no es una letra o un caracter de subrayado, se comprueba si se está procesando un identificador
            if (index > 0 && es_identificador) {
                index = 0;
                es_identificador = 0;

                // Se comprueba si el identificador es una palabra reservada
                bool es_palabra_reservada = false;
                for (const char **word = palabra_reservada; *word!= NULL; word++) {
                    if (strcmp(*word, identificador) == 0) {
                        es_palabra_reservada = true;
                        break;
                    }
                }

                // Si no es una palabra reservada, se guarda en el arreglo de identificadores
                if (!es_palabra_reservada) {
                    //strdup crea una copia de la palabra y devuelve un puntero a ella
                    identificadores_encontrados[num_identificadores++] = strdup(identificador);
                }
            }
        }
    }
}

//Funcion que encuentra los n�meros (0 - 9) y los guarda para imprimierlos posteriormente
void busca_numeros(FILE * archivo){
	int numero;
	//int bandera = 0; // Flag para indicar si estamos dentro de un n�mero
	while ((numero = fgetc(archivo)) != EOF) {
		if (isdigit(numero)) {
			numero = numero - 48;
			numeros_encontrados[num_numeros++] = numero;
		}
	}
}

//Funcion que encuentra los operadores (+ - / * < > =) y los guarda para imprimierlos posteriormente
void busca_operadores(FILE * archivo){
    int caracter;
    while ((caracter = fgetc(archivo))!= EOF) {
        if (caracter == '+' || caracter == '-' || caracter == '*' || caracter == '/' || caracter == '<' || caracter == '>' || caracter == '=') {
            operadores_encontrados[num_operadores++] = (char) caracter;
        }
    }
}

//Funcion que encuentra los comentarios y los guarda para imprimierlos posteriormente
void busca_comentarios(FILE * archivo){
	//Identificador de comentario
	char id_comentario;
	//Cantidad de comentario en el archivo
	int num_comentarios = 0;
	//Bandera para indicar si es un comentario
	char es_comentario = 0;
	//Mientras no sea el final del archivo
	while ((id_comentario = fgetc(archivo)) != EOF) {
		switch (es_comentario){
		//Busca si es un comentario
		case 0:
			if (id_comentario == '/') {
				// Podria ser el inicio de un comentario
				int sig_carac = fgetc(archivo);
				if (sig_carac == '*') {
					//Bandera de un comentario de bloque
					es_comentario = 1; 
					//Incrementa el conteo de comentarios
					num_comentarios++;
					//Imprime el numero de comentario
					printf("\nComentario numero %i:\n", num_comentarios);
					//Imprime los id de comentarios
					putchar(id_comentario);
					putchar(sig_carac);
				} else if (sig_carac == '/') {
					//Bandera de un comentario de l�nea
					es_comentario = 2;
					//Incrementa el conteo de comentarios
					num_comentarios++;
					//Imprime el numero de comentario
					printf("\nComentario numero %i:\n", num_comentarios);	
					//Imprime los id de comentario
					putchar(id_comentario);
					putchar(sig_carac);
				} else {
					// No es un comentario, regresamos el cursor
					fseek(archivo, -1, SEEK_CUR);
				}
			}
			break;
		// Dentro de un comentario de bloque
		case 1:
			//Imprime el comentario caracter a caracter
			putchar(id_comentario);
			//Si se encunetra un * significa que el comentario puede acabar
			if (id_comentario == '*') {
				//Obtiene el siguiente caracter
				int sig_carac = fgetc(archivo);				
				//Si es / significa que el comentario acab�
				if (sig_carac == '/') {
					//Bandera de busqueda de comentario
					es_comentario = 0;
					//Imprime el id de final de comentario
					putchar(sig_carac);
				} else {
					//Regresa el cursor
					fseek(archivo, -1, SEEK_CUR);
				}
			}
			break;
		// Dentro de un comentario de linea
		case 2:
			//Imprime el comentario caracter a caracter
			putchar(id_comentario);
			//Si es \n significa que el comentario acabo
			if (id_comentario == '\n') {
				//Bandera de busqueda de comentario
				es_comentario = 0;
			}
			break;
		default:
			break;
		}
	}
	printf ("\n\nCantidad total de comentarios: %i", num_comentarios);
}

//**********************************FUNCIONES DE IMPRESION DE ELEMENTOS DEL LEXICO**********************************
//aqui solo imprimimos en pantalla las palabras reservadas encontradas
//indicamos el total encontrado y cuales fueron
void imprimir_reservadas(){
    printf("Total de palabras reservadas encontradas:%i\n",num_palabras_reservadas);
    for(int i=0;i<num_palabras_reservadas;i++){
        printf("%i : %s\n",i+1,reservadas_encontradas[i]);
    }
}

void imprimir_identificadores(){
	    printf("Total de identificadores encontrados:%i\n", num_identificadores);
    for(int i=0; i<num_identificadores; i++){
        printf("%i : %s\n", i+1, identificadores_encontrados[i]);
    }
}

void imprimir_numeros(){
	printf("Total de numeros encontrados:%i\n",num_numeros);
	for(int i = 0; i < num_numeros; i++){
		printf("%i : %i\n", i + 1, numeros_encontrados[i]);
	}
}

void imprimir_operadores(){
 printf("Total de operadores encontrados:%i\n",num_operadores);
    for(int i=0;i<num_operadores;i++){
        printf(" %i :%c \n",i+1,operadores_encontrados[i]);
    }
    printf("\n");
}