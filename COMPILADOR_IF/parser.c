/*
 * Autores: David Lozano Bañuelos
 *          Bernardo Nava Trinidad
 *          Sergio Andres Guerrero Reyes
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define MAX_LONG_TOKEN 100
#define TAM_TABLA 100
#define MAX_LONG_LINEA 254

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
void Analiza_IF(FILE *entrada,FILE *archivo_salida);
void cerrarasm(FILE * manejo);
void compiladoasm(char  *archivo);
void Lexico(FILE * archivo_entrada);
void ImprimeTablaSimbolos(const Tabla_Simbolos *tabla);
void ProcesadorLexico(FILE * archivo_entrada, Tabla_Simbolos *tabla);
int EsSeparador(char cs);
int EsReservada(const char *palabra);
void AddToken(Tabla_Simbolos *tabla, const char *token, Tipo_de_Token tipo);
void IniTablaSimbolos(Tabla_Simbolos *tabla);
//void traduce_ifs(FILE *archivo_entrada, FILE *archivo_salida);

void main(int argc, char *argv[]){
    FILE * archivo_entrada;
    FILE * archivo_salida;
    //verificamos si se esta pasando el archivo a analizar
    if(argc!=2){
        printf("\nError 0: cantidad de argumentos invalidos o archivo no encontrado.\n");
        printf("----> \"%s\" no encontrado, favor de verificar.",argv[1]);
        exit(0);
//de ser asi, checamos si es un .c, ya que es el tipo de archivo que necesitamos
    } else if( (strstr(argv[1],".c"))==NULL){
        //si no se encuentra, dara error 1, indicando que se debe revisar el nombre pasado como argumento
        //error(01,argv);
            printf("\nError 1: Archivo no encontrado o mal escrito.\n");
                printf("----> \"%s\" no se encontro o esta mal escrito.\n",argv[1]);
                printf("Te recuerdo que el programa necesita un archivo con extension .c, otro tipo dara error.");
                exit(1);
    }else{
        //intentamos abrir el archivo, el cual tiene que estar creado
        //r — abre el archivo en modo de solo lectura. 
        archivo_entrada=fopen(argv[1],"r");
        //"w+" : crear un archivo para lectura y escritura, se crea si no existe o se sobreescribe si existe.
	    //se optp por eso para obtener una salida siempre actualizada...
        archivo_salida=fopen("compilado.asm","w+");
        if(!archivo_entrada)
            error(01,argv);        
            if(!archivo_salida){
               printf("\nError 2: No se pudo acceder o crear el archivo solicitado.\n");
                exit(2);
        //si se pudo leer el archivo de entrada y se crea el archivo de salida, podemos continuar
        }else{
            printf("Archivo encontrado, abriendo...\n");
            Lexico(archivo_entrada);
//se debe tener precaucion con lo que se pone en el archivo de salida, ya que no reemplaza lo que contiene el documento
            rewind(archivo_entrada);
			     Analiza_IF(archivo_entrada,archivo_salida);
			      cerrarasm(archivo_salida);
			      fclose(archivo_entrada);
			      fclose(archivo_salida);
            compiladoasm("compilado.asm");
        }
    }
}

//
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
        }
}

//funcion con comandos para cerrar el archivo .asm
void cerrarasm(FILE * manejo){
//se agregan hasta el final del archivo las lineas 
//mov eax,1
//int 80h
fseek(manejo,0,SEEK_END);
fwrite("\n",1,strlen("\n"),manejo);
fwrite("done:",1,strlen("done:"),manejo);
fwrite("\n",1,strlen("\n"),manejo);
fwrite("    mov eax,1\n",1,strlen("    mov eax,1\n"), manejo);
fwrite("    mov ebx,0\n",1,strlen("    mov ebx,0\n"), manejo);
fwrite("    int 80h\n",1,strlen("    int 80h\n"), manejo);
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
    system("./compilado");
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
/*
void traduce_ifs(FILE *entrada, FILE *archivo_salida) {
  char linea[1024];
  int condicion = 0;
  int i=0,conteo=0,contelse = 0,flag=0;
  char TokenActual[MAX_LONG_TOKEN];
  int indToken = 0;
  // Inicializa el buffer para el token actual
  TokenActual[0] = '\0';
  // Lee la primera línea del archivo de entrada
  if (fgets(linea, sizeof(linea), entrada) == NULL) {
    fprintf(stderr, "Error al leer el archivo de entrada\n");
    return;
  }

  // Ciclo para procesar cada línea del archivo de entrada
  while (linea[0] != '\0') {
    // Elimina el carácter de nueva línea y otros espacios en blanco al final de la línea
    char *lineaSinN = strtok(linea, "\n\r");

    // Verifica si la línea contiene la palabra clave "if"
    if (strstr(lineaSinN, "if") != NULL) {
      conteo++;
    
      condicion = 1;
      i = 0;
    } else if (strstr(lineaSinN, "else") != NULL) {
      contelse++;
      condicion = 0;
      i = 0;
    } else if (condicion == 1) {
      flag++;
    }
    // Lee la siguiente línea del archivo de entrada
    if (fgets(linea, sizeof(linea),entrada) == NULL) {
      break;
    }
  }

    if(conteo==1&& contelse==0){
      fprintf(archivo_salida,"section .data\n");
      fprintf(archivo_salida,"  a dd 1\n");
      fprintf(archivo_salida,"  b dd 1\n");
      fprintf(archivo_salida,"  msg db 'positivo',0\n");
      fprintf(archivo_salida,"  len equ $ - msg\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"section .text\n");
      fprintf(archivo_salida,"  global _start\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"_start:\n");
      fprintf(archivo_salida,"  mov eax, [a];\n");
      fprintf(archivo_salida,"  mov ebx, [b];\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"cmp eax,ebx;\n");
      fprintf(archivo_salida,"je if1;\n");
      fprintf(archivo_salida,"jne done;\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"if1:\n");
      fprintf(archivo_salida,"  mov eax, 4;\n");
      fprintf(archivo_salida,"  mov ebx, 1;\n");
      fprintf(archivo_salida,"  mov ecx, msg;\n");
      fprintf(archivo_salida,"  mov edx, leng;\n");
      fprintf(archivo_salida,"  int 80h;\n");
      fprintf(archivo_salida,"  jmp done;");
    } else if(conteo==1 && contelse==1){
      fprintf(archivo_salida,"section .data\n");
      fprintf(archivo_salida,"  a dd 1\n");
      fprintf(archivo_salida,"  b dd 1\n");
      fprintf(archivo_salida,"  msg db 'positivo',0\n");
      fprintf(archivo_salida,"  len equ $ - msg\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"section .text\n");
      fprintf(archivo_salida,"  global _start\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"_start:\n");
      fprintf(archivo_salida,"  mov eax, [a];\n");
      fprintf(archivo_salida,"  mov ebx, [b];\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"cmp eax,ebx;\n");
      fprintf(archivo_salida,"je if1;\n");
      fprintf(archivo_salida,"jne else1;\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"if1:\n");
      fprintf(archivo_salida,"  mov eax, 4;\n");
      fprintf(archivo_salida,"  mov ebx, 1;\n");
      fprintf(archivo_salida,"  mov ecx, msg;\n");
      fprintf(archivo_salida,"  mov edx, leng;\n");
      fprintf(archivo_salida,"  int 80h;\n");
      fprintf(archivo_salida,"  jmp done;\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"else1:");
      fprintf(archivo_salida,"  jmp done:\n");
      } else if(conteo==2 && contelse==1){
        fprintf(archivo_salida,"section .data\n");
        fprintf(archivo_salida,"  a dd 1\n");
        fprintf(archivo_salida,"  b dd 1\n");  
        fprintf(archivo_salida,"  c dd 1\n");
        fprintf(archivo_salida,"  msg db 'positivo',0\n");
        fprintf(archivo_salida,"  len equ $ - msg\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"section .text\n");
        fprintf(archivo_salida,"  global _start\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"_start:\n");
        fprintf(archivo_salida,"  mov eax, [a];\n");
        fprintf(archivo_salida,"  mov ebx, [b];\n");
        fprintf(archivo_salida,"  mov ecx, [c];\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"cmp eax,ebx;\n");
        fprintf(archivo_salida,"je if1;\n");
        fprintf(archivo_salida,"jmp done;\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"if1:\n");
        fprintf(archivo_salida,"  cmp edx,ecx;\n");
        fprintf(archivo_salida,"  je if2;\n");
        fprintf(archivo_salida,"  jne else1;\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"if2:\n");
        fprintf(archivo_salida,"  mov eax, 4;\n");
        fprintf(archivo_salida,"  mov ebx, 1;\n");
        fprintf(archivo_salida,"  mov ecx, msg;\n");
        fprintf(archivo_salida,"  mov edx, len;\n");
        fprintf(archivo_salida,"  int 80h;\n");
        fprintf(archivo_salida,"  jmp done;\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"else1:\n");
        fprintf(archivo_salida,"  jmp done;\n");
    } else if(conteo==2 && contelse==2){
        fprintf(archivo_salida,"section .data\n");
        fprintf(archivo_salida,"  a dd 1\n");
        fprintf(archivo_salida,"  b dd 1\n");  
        fprintf(archivo_salida,"  c dd 1\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"section .text\n");
        fprintf(archivo_salida,"  global _start\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"_start:\n");
        fprintf(archivo_salida,"  mov eax, [a];\n");
        fprintf(archivo_salida,"  mov ebx, [b];\n");
        fprintf(archivo_salida,"  mov ecx, [c];\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"cmp eax,ebx;\n");
        fprintf(archivo_salida,"je if1;\n");
        fprintf(archivo_salida,"jne else1;\n");
        fprintf(archivo_salida,"if1:\n");
        fprintf(archivo_salida,"  cmp edx,ecx;\n");
        fprintf(archivo_salida,"  je if2;\n"); 
        fprintf(archivo_salida,"  jne else2;\n");
        fprintf(archivo_salida,"if2;\n");
        fprintf(archivo_salida,"  mov eax, 4;\n");
        fprintf(archivo_salida,"  mov ebx, 1;\n");
        fprintf(archivo_salida,"  mov ecx, msg;\n");
        fprintf(archivo_salida,"  mov edx, len;\n");
        fprintf(archivo_salida,"  int 80h;\n");
        fprintf(archivo_salida,"  jmp done;\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"else1:\n");
        fprintf(archivo_salida,"  jmp done;\n");
        fprintf(archivo_salida,"else2:\n");
        fprintf(archivo_salida,"  jmp done;\n");
    } else{
        fprintf(archivo_salida,"section .text\n");
        fprintf(archivo_salida,"  global _start\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"_start:\n");
        fprintf(archivo_salida,"   jmp done;\n");
    }
}
*/
void Analiza_IF(FILE *entrada,FILE *archivo_salida){
	
	char linea[MAX_LONG_LINEA];
	int conteo_if = 0, conteo_else = 0;
	int numero_linea = 0,conteoif=0,conteoelse=0;
	char mensaje_error[MAX_LONG_LINEA] = "";
	
	
	// Lee el archivo l�nea por l�nea
	while (fgets(linea, MAX_LONG_LINEA, entrada)) {
		numero_linea++;
		
		// Se salta espacios al inicio de la l�nea
		char *inicio = linea;
		while (*inicio == ' ' || *inicio == '\t') {
			inicio++;
		}
		
		// Verifica si hay una l�nea que contiene un 'if'
		if (strncmp(inicio, "if(", 3) == 0 || strncmp(inicio, "if (", 4) == 0) {
			conteo_if++;
      conteoif++;
			while (!strchr(inicio, '{')) {
				inicio++;
				if (strchr(inicio, '}')){
					snprintf(mensaje_error, sizeof(mensaje_error), "Error: se esperaba un '{' despues del 'if' en la linea %d", numero_linea);
					break;
				}
			}
		}

		// Verifica si hay una l�nea que contiene un 'else'
		if (strncmp(inicio, "}else", 5) == 0 || strncmp(inicio, "else", 4) == 0) {
			conteo_else++;
      conteoelse++;
			if (conteo_if == 0) {
				snprintf(mensaje_error, sizeof(mensaje_error), "Error: 'else' sin correspondiente 'if' en la l�nea %d", numero_linea);
				break;
			}
			while (!strchr(inicio, '{')) {
				inicio++;
				if (strchr(inicio, '}')){
					snprintf(mensaje_error, sizeof(mensaje_error), "Error: se esperaba un '{' despues del 'else' en la linea %d", numero_linea);
					break;
				}
			}
		}
		// Verifica si hay una l�nea que contiene un '}' de cierre
		if (strchr(inicio, '}')) {
			if (conteo_else > 0) {
				conteo_else--;
				conteo_if--;
			} else if (conteo_if > 0) {
				conteo_if--;
			}
		}
	}
    printf("%d,%d\n",conteoif,conteoelse);
    if(conteoif==1&& conteoelse==0){
      fprintf(archivo_salida,"section .data\n");
      fprintf(archivo_salida,"  a dd 1\n");
      fprintf(archivo_salida,"  b dd 1\n");
      fprintf(archivo_salida,"  msg db 'positivo',0\n");
      fprintf(archivo_salida,"  len equ $ - msg\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"section .text\n");
      fprintf(archivo_salida,"  global _start\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"_start:\n");
      fprintf(archivo_salida,"  mov eax, [a];\n");
      fprintf(archivo_salida,"  mov ebx, [b];\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"cmp eax,ebx;\n");
      fprintf(archivo_salida,"je if1;\n");
      fprintf(archivo_salida,"jne done;\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"if1:\n");
      fprintf(archivo_salida,"  mov eax, 4;\n");
      fprintf(archivo_salida,"  mov ebx, 1;\n");
      fprintf(archivo_salida,"  mov ecx, msg;\n");
      fprintf(archivo_salida,"  mov edx, len;\n");
      fprintf(archivo_salida,"  int 80h;\n");
      fprintf(archivo_salida,"  jmp done;");
    } else if(conteoif==1 && conteoelse==1){
      fprintf(archivo_salida,"section .data\n");
      fprintf(archivo_salida,"  a dd 1\n");
      fprintf(archivo_salida,"  b dd 1\n");
      fprintf(archivo_salida,"  msg db 'positivo',0\n");
      fprintf(archivo_salida,"  len equ $ - msg\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"section .text\n");
      fprintf(archivo_salida,"  global _start\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"_start:\n");
      fprintf(archivo_salida,"  mov eax, [a];\n");
      fprintf(archivo_salida,"  mov ebx, [b];\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"cmp eax,ebx;\n");
      fprintf(archivo_salida,"je if1;\n");
      fprintf(archivo_salida,"jne else1;\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"if1:\n");
      fprintf(archivo_salida,"  mov eax, 4;\n");
      fprintf(archivo_salida,"  mov ebx, 1;\n");
      fprintf(archivo_salida,"  mov ecx, msg;\n");
      fprintf(archivo_salida,"  mov edx, leng;\n");
      fprintf(archivo_salida,"  int 80h;\n");
      fprintf(archivo_salida,"  jmp done;\n");
      fprintf(archivo_salida,"\n");
      fprintf(archivo_salida,"else1:");
      fprintf(archivo_salida,"  jmp done:\n");
      } else if(conteoif==2 && conteoelse==1){
        fprintf(archivo_salida,"section .data\n");
        fprintf(archivo_salida,"  a dd 1\n");
        fprintf(archivo_salida,"  b dd 1\n");  
        fprintf(archivo_salida,"  c dd 1\n");
        fprintf(archivo_salida,"  msg db 'positivo',0\n");
        fprintf(archivo_salida,"  len equ $ - msg\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"section .text\n");
        fprintf(archivo_salida,"  global _start\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"_start:\n");
        fprintf(archivo_salida,"  mov eax, [a];\n");
        fprintf(archivo_salida,"  mov ebx, [b];\n");
        fprintf(archivo_salida,"  mov ecx, [c];\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"cmp eax,ebx;\n");
        fprintf(archivo_salida,"je if1;\n");
        fprintf(archivo_salida,"jmp done;\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"if1:\n");
        fprintf(archivo_salida,"  cmp edx,ecx;\n");
        fprintf(archivo_salida,"  je if2;\n");
        fprintf(archivo_salida,"  jne else1;\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"if2:\n");
        fprintf(archivo_salida,"  mov eax, 4;\n");
        fprintf(archivo_salida,"  mov ebx, 1;\n");
        fprintf(archivo_salida,"  mov ecx, msg;\n");
        fprintf(archivo_salida,"  mov edx, len;\n");
        fprintf(archivo_salida,"  int 80h;\n");
        fprintf(archivo_salida,"  jmp done;\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"else1:\n");
        fprintf(archivo_salida,"  jmp done;\n");
    } else if(conteoif==2 && conteoelse==2){
        fprintf(archivo_salida,"section .data\n");
        fprintf(archivo_salida,"  a dd 1\n");
        fprintf(archivo_salida,"  b dd 1\n");  
        fprintf(archivo_salida,"  c dd 1\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"section .text\n");
        fprintf(archivo_salida,"  global _start\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"_start:\n");
        fprintf(archivo_salida,"  mov eax, [a];\n");
        fprintf(archivo_salida,"  mov ebx, [b];\n");
        fprintf(archivo_salida,"  mov ecx, [c];\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"cmp eax,ebx;\n");
        fprintf(archivo_salida,"je if1;\n");
        fprintf(archivo_salida,"jne else1;\n");
        fprintf(archivo_salida,"if1:\n");
        fprintf(archivo_salida,"  cmp edx,ecx;\n");
        fprintf(archivo_salida,"  je if2;\n"); 
        fprintf(archivo_salida,"  jne else2;\n");
        fprintf(archivo_salida,"if2;\n");
        fprintf(archivo_salida,"  mov eax, 4;\n");
        fprintf(archivo_salida,"  mov ebx, 1;\n");
        fprintf(archivo_salida,"  mov ecx, msg;\n");
        fprintf(archivo_salida,"  mov edx, len;\n");
        fprintf(archivo_salida,"  int 80h;\n");
        fprintf(archivo_salida,"  jmp done;\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"else1:\n");
        fprintf(archivo_salida,"  jmp done;\n");
        fprintf(archivo_salida,"else2:\n");
        fprintf(archivo_salida,"  jmp done;\n");
    } else{
        fprintf(archivo_salida,"section .text\n");
        fprintf(archivo_salida,"  global _start\n");
        fprintf(archivo_salida,"\n");
        fprintf(archivo_salida,"_start:\n");
        fprintf(archivo_salida,"   jmp done;\n");
    }
   
	// Verifica si hay alg�n error
	if (strlen(mensaje_error) > 0) {
		printf("%s\n", mensaje_error);
	} else if (conteo_if > 0) {
		printf("Error: Bloques 'if' sin cerrar en la linea %d\n", numero_linea);
	} else if (conteo_else > 0) {
		printf("Error: Bloques 'else' sin cerrar\n");
	} else {
		  printf("La estructura de los 'if' y 'else' es correcta\n");
     
  }
}
