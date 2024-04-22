#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
//ubicacion del puntero, para el manejo de errores
int linea=1,columna=1;
//columna es la posicion en la linea
// y la linea es cada linea del documento

//declaracion de funciones para usarse en el documento
void errores(int linea, int columna);

int main(int argc, char *argv[]){
    FILE * archivo;
    char caracter;
    char expresion[25];
    int i;
    archivo=fopen(argv[1],"r");
    if(!archivo){ //<-- si el archivo no existe dara error
        printf("archivo no existente:%s",argv[1]);
        return 1;
    }else{
        //el archivo existe, por lo tanto leeremos lo que contiene
       for(columna=0;(caracter=fgetc(archivo))!=EOF;columna++){
        expresion[columna]=caracter; 
       }
       expresion[columna]='\0';
       fclose(archivo); //liberamos el archivo y procedemos a analizar la expresion obtenida

       for(columna=0;expresion[columna]!='\0';columna++){
        if(expresion[columna]=='x'){
            columna++;
            if((expresion[columna]=='+') || (expresion[columna]=='-') || (expresion[columna]=='*') || (expresion[columna]=='/') ){
                columna++;
                if(expresion[columna]=='x'){
                    columna++;
                    if( (expresion[columna]=='+') || (expresion[columna]=='-') || (expresion[columna]=='*') || (expresion[columna]=='/') ){
                        columna++;
                        if(expresion[columna]=='x'){
                            columna++;
                            if( (expresion[columna]=='+') || (expresion[columna]=='-') || (expresion[columna]=='*') || (expresion[columna]=='/') ){
                                columna++;
                                if(expresion[columna]=='x'){
                                    printf("sintaxis valida\n");
                                    break;
                                }else{
                                    printf("Sintaxis invalida");
                                    errores(linea,columna);
                                    break;
                                }
                            }else{
                                if(expresion[columna]==EOF || expresion[columna]=='\0'){
                                    printf("Sintaxis valida");
                                    break;
                                }else{
                                    printf("Sintaxis invalida");
                                    errores(linea,columna);
                                    break;
                                }
                            }
                        }else{
                            printf("Sintaxis invalida");
                            errores(linea,columna);
                            break;
                        }
                    }else{
                        if(expresion[columna]==EOF || expresion[columna]=='\0'){
                            printf("Sintaxis Valida");
                            break;
                        }else{
                            printf("Sintaxis invalida");
                            errores(linea,columna);
                            break;
                        }
                    }
                }else{
                    printf("Sintaxis invalida");
                    errores(linea,columna);
                    break;
                 }
            }else{
                printf("Sintaxis invalida");
                errores(linea,columna);
                break;
            }
        } else if(expresion[columna+1]=='\0' || expresion[columna+1]==EOF){
                printf("Sintaxis valida");
                }else{
                    errores(linea,columna);
                    break;
                }
        }
    }
    printf("\nExpresion leida: %s",expresion);
    return 0;
}

void errores(int linea, int columna){
    printf("\nError en la pos: (%d,%d)\n",linea,columna);
}
