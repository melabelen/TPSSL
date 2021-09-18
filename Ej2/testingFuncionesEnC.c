/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Definicion del stack Int*/

typedef struct nodoInt{	
    int info;
	struct nodoInt* sgte;
} nodoInt;

void pushInt(nodoInt**, int);
int popInt(nodoInt**);

int popInt(nodoInt** pila){
    int x;
    nodoInt *p = *pila; 
    if ( !p ){
        return -1;
    }
    x = p->info;     
    *pila = p->sgte;    
    p->sgte = NULL; 
    free( p );       
    
	return x;
}


void pushInt(nodoInt** pila, int x){	

 nodoInt*p = malloc(sizeof (nodoInt));
  p->info = x;	
  p->sgte = *pila;	
  *pila = p;		
  return;
}


/*Fin Definicion stack Int*/


/*Definicion del stack String*/

typedef struct nodoString{	
    char* info;
	struct nodoString* sgte;
} nodoString;

void pushString(nodoString**, char*);
char* popString(nodoString**);

char* popString(nodoString** pila){
    char* x;
    nodoString *p = *pila; 
    if ( !p ){
        return "-1";
    }
    x = p->info;     
    *pila = p->sgte;    
    p->sgte = NULL; 
    free( p );       
    
	return x;
}


void pushString(nodoString** pila, char* x){		

 nodoString*p = malloc(sizeof (nodoString));
  p->info = x;	
  p->sgte = *pila;	
  *pila = p;		
  return;
}

/*Fin Definicion stack String*/


int main()
{
    char* palabraAnalizada = "2+2";
    nodoString* stack = NULL;
    obtenerNotacion(palabraAnalizada, &stack);
     for (int k = 0; k < strlen(palabraAnalizada); k++){
                  printf("%s", popString(&stack));
                }
    return 0;
}

void pushearOperador(char* operadores, nodoString** stack){
    int i = 0;
    while(i < strlen(operadores)){
        pushString( &stack, operadores[i]);
        i++;
    }
}

void obtenerNotacion(char* palabraAnalizada, nodoString** stack){
  int i = 0;
  char*notacion = "";
  char* numeroActual = "";
  char* operadorEnEspera = ""; 
  char* operadorAuxiliar = "*";
  // analizo la operación que ya sé que es válida
  while(palabraAnalizada[i] != '\0'){
        //no es un operador, es un numero y obtengo todos sus digitos;
        if(palabraAnalizada[i] != '+' && palabraAnalizada[i] != '*' && palabraAnalizada[i] != '-'){
            strcat(numeroActual,palabraAnalizada[i]);
            i = i +1;
        }
        //es un operador
        else{
          //como es un operador, el numero que lo precede ya es parte de la notacion
          pushString(&stack, numeroActual);
          //reseteo el numeroActual
            for (int k = 0; k < i; k++){
                  numeroActual[k] = '\0';
                }
          //El operador es de los de menor prioridad
          if(operadorEnEspera == "+" || operadorEnEspera == '-'){
            //verifico si el operador actual tiene mayor prioridad
              if(palabraAnalizada[i] == '*'){
                //la notacion toma al operador de mayor prioridad y el de menor
                //sigue en espera
                strcat(operadorAuxiliar,operadorEnEspera);
                strcat(operadorEnEspera,operadorAuxiliar);
              }
              else{
                //el operador en espera pasa a ser parte de la notacion 
                //el operador analizado actualmente pasa a estar en espera
                pushearOperador(operadorEnEspera, stack);
                operadorEnEspera = palabraAnalizada[i];
              }
          }
          //el operador es de mayor prioridad, el caracter vacio o
          //un conjunto de operadores
          else{
              if(palabraAnalizada[i] == '*'){
                pushearOperador('*', stack);
              }
                else{
                  pushearOperador(operadorEnEspera, stack);
                   operadorEnEspera = palabraAnalizada[i];
                }
          }
          i = i + 1;
          operadorAuxiliar = '*';
        }
  }
  //guardo lo que quedó en las variables al terminar el loop
  pushString(&stack, numeroActual);
  pushearOperador(operadorEnEspera, &stack);

}


// int main()
// {
//     //32*
//     nodoString* stackNotacion = NULL;
//     pushString(&stackNotacion, "3");
//     pushString(&stackNotacion, "2");
//     pushString(&stackNotacion, "*");
    
//     int resultado = evaluarNotacion(stackNotacion);

//     return 0;
// }


int evaluarNotacion (nodoString** tokens) {
    int i = 0;
    nodoInt* stack = NULL;
    char* nodoActual = popString(&tokens);
    int operador1 = 0;
    int operador2 = 0;

    while(nodoActual != "-1") {
        if (nodoActual == '+' || nodoActual == '*' || nodoActual == '-') {
          operador2 = popInt(&stack);
          operador1 = popInt(&stack);
          pushInt(&stack, evaluar(operador1, operador2, nodoActual));
        } else {
          pushInt(&stack, atoi(nodoActual));
        }
        nodoActual = popString(&tokens);
  }

  return popInt(&stack);
};

int evaluar(n1, n2, operador){
  if (operador == '+') return n1 + n2;
  if (operador == '-') return n1 - n2;
  if (operador == '*') return n1 * n2;
};


