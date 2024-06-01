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

void IniTablaSimbolos(Tabla_Simbolos *tabla){
	tabla -> c = 0;
}

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


void ProcesadorLexico(const char *archivo_entrada, Tabla_Simbolos *tabla){
	FILE *entrada = fopen(archivo_entrada, "r");
	
	if(!entrada){
		printf("No se pudo abrir el archivo %s\n", entrada);
		return;
	}
	
	char e;
	char TokenActual[MAX_LONG_TOKEN];
	int indToken = 0;
	
	while ((e = fgetc(entrada)) != EOF){
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
	
	fclose(entrada);
}

void ImprimeTablaSimbolos(const Tabla_Simbolos *tabla){
	printf("\tTabla de Simbolos\n");
	printf("--------------------------------------------------\n");
	printf("\tTOKEN\t|\tTIPO\n");
	printf("--------------------------------------------------\n");
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

void Lexico(const char *archivo_entrada){
	Tabla_Simbolos tabla;
	
	IniTablaSimbolos(&tabla);
	ProcesadorLexico(archivo_entrada, &tabla);
	ImprimeTablaSimbolos(&tabla);
}


int main(int argc, char *argv[]){
	const char *entrada = argv[1];

	Lexico(entrada);
	return 0;
}
