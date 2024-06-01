/*
 * Autores: David Lozano Bañuelos
 *          Bernardo Nava Trinidad
 *          Sergio Andres Guerrero Reyes
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LONG_TOKEN 100
#define TAM_TABLA 100

typedef enum{
	RESERVADA,
	SEPARADOR,
	IDENTIFICADOR
}Tipo_de_Token;

typedef struct {
	char token[MAX_LONG_TOKEN];
	Tipo_de_Token tipo; 
}Token;

typedef struct{
	Token tokens[TAM_TABLA];
	int c;
}Tabla_Simbolos;

//declaracion de funciones a usar en todo el codigo
void error(int tipo, char **manejo);
void cerrarasm(FILE * manejo);
void compiladoasm(char  *archivo);
void Lexico(FILE * archivo_entrada);
void ImprimeTablaSimbolos(const Tabla_Simbolos *tabla);
void ProcesadorLexico(FILE * archivo_entrada, Tabla_Simbolos *tabla);
int EsSeparador(char cs);
int EsReservada(const char *palabra);
void AddToken(Tabla_Simbolos *tabla, const char *token, Tipo_de_Token tipo);
void IniTablaSimbolos(Tabla_Simbolos *tabla);


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
            Lexico(archivo_entrada);
            fclose(archivo_entrada);
            cerrarasm(archivo_salida);
            fclose(archivo_salida);
            compiladoasm("compilado.asm");
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------
//                                              FUNCIONES
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

//funcion con comandos para cerrar el archivo .asm
void cerrarasm(FILE * manejo){
//se agregan hasta el final del archivo las lineas 
//mov eax,1
//int 80h
fseek(manejo,0,SEEK_END);
fwrite("mov eax,1\n",1,strlen("mov eax,1\n"), manejo);
fwrite("mov ebx,0\n",1,strlen("mov ebx,0\n"), manejo);
fwrite("int 80h\n",1,strlen("int 80h\n"), manejo);
}
//funcion para el compilado del archivo .asm
//a travez de comandos ingresados con EXECVP compilamos el .asm
void compiladoasm(char *archivo) {
    const char *const args[] = {"nasm", "-f", "elf64", archivo, NULL};
    const char *const more[]= {"ld","compilado.o","-o","compilado"};
    if (execvp(args[0], args) == -1) {
        perror("\nexecvp");
        exit(1);
    }
    if (execvp(more[0], more) == -1) {
        perror("\nexecvp");
        exit(1);
    }
    //system("./compilado");
}
//Inicio de la  tabla de simbolos
void IniTablaSimbolos(Tabla_Simbolos *tabla){
	tabla -> c = 0;
}

//funcion para añadir tokens, dara error si se llena
void AddToken(Tabla_Simbolos *tabla, const char *token, Tipo_de_Token tipo){
	if(tabla -> c < TAM_TABLA){
		strcpy(tabla -> tokens[tabla -> c].token, token);
		tabla -> tokens [tabla -> c].tipo = tipo;
		tabla -> c++;
	}
	else{
		printf("Tabla de simbolos llena!\n");
	}
}
//funcion para comprobar si el token encontrado es palabra reservada o no
int EsReservada(const char *palabra){
	const char *PalabrasReservadas[] = {
		"if",
		"else",
		"int",
		"#include",
		"<stdlib.h>",
		"<stddio.h>",
		"main",
		"return",
		"printf"
	};
	int nPalabrasReservadas = sizeof(PalabrasReservadas) / sizeof(PalabrasReservadas[0]);
	
	for (int i = 0; i < nPalabrasReservadas; i++){
		if(strcmp(palabra, PalabrasReservadas[i]) == 0){
			return 1;
		}
	}
	return 0;
}
//funcion para saber si el token encontrado es un separador 
int EsSeparador(char cs){
	char Separadores[] = {
		' ',
		'\t',
		'\n',
		';',
		',',
		'.',
		'(',
		')',
		'[',
		']',
		'{',
		'}'
	};
	int nSeparadores = sizeof(Separadores) / sizeof(Separadores[0]);
	
	for (int i = 0; i < nSeparadores; i++){
		if(cs == Separadores[i]){
			return 1;
		}
	}
	return 0;
}

//Funcion que lee caracter por caracter el archivo que le pasemos y compara el token encontrado
void ProcesadorLexico(FILE *archivo_entrada, Tabla_Simbolos *tabla){
	if(archivo_entrada==NULL){
		printf("No se pudo abrir el archivo: %s\n", archivo_entrada);
		return;
	}
	
	char e;
	char TokenActual[MAX_LONG_TOKEN];
	int indToken = 0;
	
	while ((e = fgetc(archivo_entrada)) != EOF){
		if (EsSeparador(e)){
			if (indToken > 0){
				TokenActual[indToken] = '\0';
				if (EsReservada(TokenActual)){
					AddToken(tabla, TokenActual, RESERVADA);
				}else{
					AddToken(tabla, TokenActual, IDENTIFICADOR);
				}
				indToken = 0;
			}
			if(e != ' ' && e != '\t' && e != '\n'){
				char sepToken[2] = {e, '\0'};
				AddToken(tabla, sepToken, SEPARADOR);
			}
		}else{
			TokenActual[indToken++] = e;
		}
	}
	
	if (indToken > 0){
		TokenActual[indToken] = '\0';
		if (EsReservada(TokenActual)){
			AddToken(tabla, TokenActual, RESERVADA);
		}else{
			AddToken(tabla, TokenActual, IDENTIFICADOR);
		}
	}
	
	fclose(archivo_entrada);
}
//funcion para imprimir la tabla de simbolos
void ImprimeTablaSimbolos(const Tabla_Simbolos *tabla){
	printf("\tTabla de Simbolos\n");
	printf("--------------------------------------------------\n");
	printf("\tTOKEN\t|\tTIPO\n");
	for (int i = 0; i < tabla -> c; i++){
		printf("%s\t\t|\t", tabla -> tokens[i].token);
		switch (tabla -> tokens[i].tipo){
			case RESERVADA:
				printf("Palabra Reservada\n");
				break;
				
			case SEPARADOR:
				printf("Separador\n");
				break;
				
			case IDENTIFICADOR:
				printf("Identificador\n");
				break;
				
			default:
				printf("Desconocido\n");
				break;
		}
		printf("--------------------------------------------------\n");
	}
}
//Funcion para poder empezar con la etapa del lexico
void Lexico(FILE * archivo_entrada){
	Tabla_Simbolos tabla;
	
	IniTablaSimbolos(&tabla);
	ProcesadorLexico(archivo_entrada, &tabla);
	ImprimeTablaSimbolos(&tabla);
}