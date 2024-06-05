#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LONG_LINEA 254

void Analiza_IF(const char *entrada){
	FILE *archivo_entrada = fopen(entrada, "r");
	
	char linea[MAX_LONG_LINEA];
	int conteo_if = 0, conteo_else = 0;
	int numero_linea = 0;
	char mensaje_error[MAX_LONG_LINEA] = "";
	
	
	if(!archivo_entrada){
		printf("No se pudo abrir el archivo %s\n", entrada);
		return;
	}
	
	// Lee el archivo línea por línea
	while (fgets(linea, MAX_LONG_LINEA, archivo_entrada)) {
		numero_linea++;
		
		// Se salta espacios al inicio de la línea
		char *inicio = linea;
		while (*inicio == ' ' || *inicio == '\t') {
			inicio++;
		}
		
		// Verifica si hay una línea que contiene un 'if'
		if (strncmp(inicio, "if(", 3) == 0 || strncmp(inicio, "if (", 4) == 0) {
			conteo_if++;
			printf("Verifica if: %d\n", conteo_if);
			while (!strchr(inicio, '{')) {
				inicio++;
				if (strchr(inicio, '}')){
					snprintf(mensaje_error, sizeof(mensaje_error), "Error: se esperaba un '{' despues del 'if' en la linea %d", numero_linea);
					break;
				}
			}
		}
		
		// Verifica si hay una línea que contiene un 'else'
		if (strncmp(inicio, "}else", 5) == 0 || strncmp(inicio, "else", 4) == 0) {
			conteo_else++;
			printf("Verifica else: %d\n", conteo_else);
			if (conteo_if == 0) {
				snprintf(mensaje_error, sizeof(mensaje_error), "Error: 'else' sin correspondiente 'if' en la línea %d", numero_linea);
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
		
		
		// Verifica si hay una línea que contiene un '}' de cierre
		if (strchr(inicio, '}')) {
			if (conteo_else > 0) {
				conteo_else--;
				conteo_if--;
				printf("Verifica else }: %d %d\n", conteo_if, conteo_else);
			} else if (conteo_if > 0) {
				conteo_if--;
				printf("Verifica if }: %d\n", conteo_if);
			}
		}
	}
	
	printf("Verifica final: %d %d\n", conteo_if, conteo_else);
	// Limpia
	fclose(archivo_entrada);
	
	// Verifica si hay algún error
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

int main(int argc, char *argv[]) {
	
	const char *entrada = argv[1];
	Analiza_IF(entrada);
	
	return 0;
}
