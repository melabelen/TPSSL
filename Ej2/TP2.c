#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tablaDeTransicion[4][4] = {
    {1, 3, 3, 3},
    {1, 2, 1, 3},
    {1, 3, 3, 3},
    {3, 3, 3, 3},
};

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

void pushString(nodoString*, char*);
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

/*Firma de funciones*/
int procesarPalaba (char palabraRecibida[], int largoPalabra);
void actualizarEstado (int columnaAnterior, int *filaAnterior);
int determinarColumna (char letra);
void grabarPalabra (char palabraAnalizada[], int tipoDeConstate, FILE *fSalida);
void obtenerNotacion(char* palabraAnalizada, nodoString** stack);
void pushearOperador(char* operadores, nodoString** stack);
int resolverNotacion (nodoString** tokens);
int evaluar(int n1,int n2,char* operador);
int sacarCalculo(char* palabraAnalizada);

/*Fin firma de funciones*/



int main(){
   //se abre el archivo de lectura
   FILE *fEntrada = fopen("Entrada.txt", "r");// o la posicion de disco duro en donde lo tengan guardado
   //se abre el archivo de escritura
   FILE *fSalida = fopen("Salida.txt", "w");

    char letra;
    int j = 0, i = 0, k = 0;
    char palabraAux[150]={""};
    while (!feof(fEntrada)){
        letra = fgetc(fEntrada);
         if (letra != '&' && !feof(fEntrada)){
              palabraAux[i] = letra;
              i++;
         }else{ //Termina la palabra

                 //proceso palabraAux
                char Tipo=procesarPalabra(palabraAux,i);

                //Se graba la palabra en el archivo de salida
                grabarPalabra(palabraAux,Tipo,fSalida);

                //Se reincializa palabra
                for (k = 0; k < i; k++){
                  palabraAux[k] = '\0';
                }
                //Reinicializo contador
                i=0;
                
         }
      }

   fclose(fEntrada);
   fclose(fSalida);
   printf("%s", "Procesamiento Terminado");
   return 0;
}


void grabarPalabra (char palabraAnalizada[], int tipoDeConstate, FILE *fSalida){
    fseek(fSalida, 0, SEEK_END); //Nos paramos al final del archivo
    char constanteEntera[20];
    if (tipoDeConstate == 0) {
      fprintf (fSalida, "%s \t\t %s\n", palabraAnalizada, "El calculo es invalido"); 
    }
    else if (tipoDeConstate == 1){
        int resultado = sacarCalculo(palabraAnalizada);
      fprintf (fSalida, "%s \t\t El calculo es válido y el resultado es %d\n", palabraAnalizada, resultado);
    }
    
};




void actualizarEstado(int columnaAnterior, int *filaAnterior){ //el estado anterior deberia estar compuesto por la columna y la fila anteriores
   int filaSiguiente = tablaDeTransicion[*filaAnterior][columnaAnterior];
   *filaAnterior = filaSiguiente;
};




int determinarColumna(char letra){
   int columnaADevolver = 0;
   switch(letra)
   {
    case '-': //Caso -
        columnaADevolver = 1;
        break;
    case '+': // Caso +
        columnaADevolver = 1;
        break;
    case '*': // caso *
        columnaADevolver = 1;
        break;
    case '0': // Caso de 0
        columnaADevolver = 2;
        break;
    case 49 ... 57: // Caso de 1-7
          columnaADevolver = 0;
        break;
       
   default:
      columnaADevolver = 3;
      break;
   }
   return columnaADevolver;
};


int procesarPalabra(char palabraRecibida[], int largoPalabra){
   int tipoDeConstante = 0; // si no se reconoce quedarÃ¡ en 0
   int columnaActual = 0,i = 0;
   int * filaActual;
   int h=0;
   filaActual= &h;
  
   
   //procesa letra por letra y va actualizando los estados
      while(palabraRecibida[i]!= '\0'){
         columnaActual=determinarColumna(palabraRecibida[i]);
         actualizarEstado(columnaActual,filaActual);

         i++;
      }

      switch(*filaActual)
      {
      case 1: 
         tipoDeConstante = 1;
         break;

         
     case 3:
        tipoDeConstante = 0;
        break;
      }
   
   return tipoDeConstante;
}

void pushearOperador(char* operadores, nodoString** stack){
    int i = 0;
    while(i < strlen(operadores)){
        pushString(stack, &operadores[i]);
        i++;
    }
}

void obtenerNotacion(char* palabraAnalizada, nodoString** stack){
  int i = 0;
  char*notacion = "";
  char numeroActual[100] = {""};
  char* operadorEnEspera = ""; 
  char* operadorAuxiliar = "*";
  char* digitoActual = "";
  // analizo la operación que ya sé que es válida
  while(strlen(palabraAnalizada) > i){
        //no es un operador, es un numero y obtengo todos sus digitos;
        if(&palabraAnalizada[i] != "+" && &palabraAnalizada[i] != "*" && &palabraAnalizada[i] != "-"){
            strcat(numeroActual,&palabraAnalizada[i]);
            i++;
        }
        //es un operador
        else{
          //como es un operador, el numero que lo precede ya es parte de la notacion
          pushString(stack, numeroActual);
          //reseteo el numeroActual
          
            for (int k = 0; k < strlen(numeroActual); k++){
                  numeroActual[k] = '\0';
                }
          //El operador es de los de menor prioridad
          if(operadorEnEspera == "+" || operadorEnEspera == "-"){
            //verifico si el operador actual tiene mayor prioridad
              if(&palabraAnalizada[i] == "*"){
                //la notacion toma al operador de mayor prioridad y el de menor
                //sigue en espera
                strcat(operadorAuxiliar,operadorEnEspera);
                strcat(operadorEnEspera,operadorAuxiliar);
              }
              else{
                //el operador en espera pasa a ser parte de la notacion 
                //el operador analizado actualmente pasa a estar en espera
                pushearOperador(operadorEnEspera, stack);
                operadorEnEspera = &palabraAnalizada[i];
              }
          }
          //el operador es de mayor prioridad, el caracter vacio o
          //un conjunto de operadores
          else{
              if(&palabraAnalizada[i] == "*"){
                pushearOperador("*", stack);
              }
                else{
                  pushearOperador(operadorEnEspera, stack);
                   operadorEnEspera = &palabraAnalizada[i];
                }
          }
          i++;
          operadorAuxiliar = "*";
        }
  }
  //guardo lo que quedó en las variables al terminar el loop
  pushString(stack, numeroActual);
  pushearOperador(operadorEnEspera, stack);

}

//funcion que resuelve la notacion polaca inversa
//recibe el stack con la notacion ya armada
int resolverNotacion (nodoString** tokens){
    int i = 0;
    //nuevo stack con el que se operará
    nodoInt* stack = NULL;
    char* nodoActual = popString(tokens);
    int operador1 = 0;
    int operador2 = 0;
    //voy tomando todos los valores del stack hasta que quede vacío
    while(nodoActual != "-1") {
        //si el token actual es un operador, entonces opero con los dos siguientes numeros del stack
        if (nodoActual == "+" || nodoActual == "*" || nodoActual == "-") {
          operador2 = popInt(&stack);
          operador1 = popInt(&stack);
          //el resultado correspondiente se agrega al stack de vuelta
          pushInt(&stack, evaluar(operador1, operador2, nodoActual));
        } else {
            // si el token es un numero, se lo agrega directo al stack
          pushInt(&stack, atoi(nodoActual));
        }
        //tomo otro valor del stack
        nodoActual = popString(tokens);
  }
    //el ultimo numero que queda en el stack en el que resolvimos la notacion es el resultado
  return popInt(&stack);
};

//funcion que resuelve las operaciones que van siendo requeridas por el stack
int evaluar(int n1,int n2,char* operador){
  if (operador == "+") return n1 + n2;
  if (operador == "-") return n1 - n2;
  if (operador == "*") return n1 * n2;
};

//el calculo se saca por notacion polaca inversa
int sacarCalculo(char* palabraAnalizada){
    nodoString* stack = NULL;
    obtenerNotacion(palabraAnalizada, &stack);
    int resultado = resolverNotacion(&stack);
    return resultado;
};
