/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int procesarPalaba (char palabraRecibida[], int largoPalabra);
void actualizarEstado (int columnaAnterior, int *filaAnterior);
int determinarColumna (char letra);
void grabarPalabra (char palabraAnalizada[], int tipoDeConstate, FILE *fSalida);

int tablaDeTransicion[7][8] = {
// Ejemplo= 017: [0][x] -> leemos letra '0' -> columna 0 -> [0][0] = 1 -> osea me voy al q1 (es decir filaSiguiente = 1).
              // [1][x] -> leemos letra '1' -> columna 1 -> [1][1] = 5 -> me voy al q5 (es decir filaSiguiente = 5).
              // [5][x] -> leemos letra '7' -> columna 1 -> [5][1] = 5 -> La posicion no es un error, y ademas por la fila y columna determinamos que es Octal.
//        		{+,-}{0}[1-7][8-9][a-fA-F][xX]{no rec}.
      /* q0  */{ 1, 2, 3, 3, 7, 7, 7},
      /* q1+ */{ 7, 7, 3, 3, 7, 7, 7}, // Estado final 1 Octal (cero)
      /* q2+ */{ 7, 6, 6, 7, 7, 4, 7}, // Estado final 2 de Decimal
      /* q3  */{ 7, 3, 3, 3, 7, 7, 7}, // Transicion de 0 a [xX]
      /* q4+ */{ 7, 5, 5, 5, 5, 7, 7}, // Estado final 4 de Hexadecimal
      /* q5+ */{ 7, 5, 5, 5, 5, 7, 7}, // Estado final 5 de Octal
      /* q6+ */{ 7, 6, 6, 7, 7, 7, 7}, // Estado final 3,6 No se reconoce
      /* q7	 */{ 7, 7, 7, 7, 7, 7, 7}  //
};

int main(){
   //se abre el archivo de lectura
   FILE *fEntrada = fopen("Entrada.txt", "r");// o la posicion de disco duro en donde lo tengan guardado
   //se abre el archivo de escritura
   FILE *fSalida = fopen("Salida.txt", "w");

    char letra;
    int j = 0, i = 0, k = 0;
    char palabraAux[150]={""};

    do {
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
      } while (!feof(fEntrada));

   fclose(fEntrada);
   fclose(fSalida);
   printf("%s", "Procesamiento Terminado");
   return 0;
}


void grabarPalabra (char palabraAnalizada[], int tipoDeConstate, FILE *fSalida){
    fseek(fSalida, 0, SEEK_END); //Nos paramos al final del archivo
    char constanteEntera[20];
    if (tipoDeConstate == 2) {
      fprintf (fSalida, "%s \t\t %s\n", palabraAnalizada, "Decimal"); 
    }
    else if (tipoDeConstate == 1){
      fprintf (fSalida, "%s \t\t %s\n", palabraAnalizada, "Octal");
    }
    else if (tipoDeConstate == 3){
      fprintf (fSalida, "%s \t\t %s\n", palabraAnalizada, "Hexadecimal");
    }
    else if (tipoDeConstate == 0){
      fprintf (fSalida, "%s \t\t %s\n", palabraAnalizada, "No Reconocida");
    }
}


void actualizarEstado(int columnaAnterior, int *filaAnterior){ //el estado anterior deberia estar compuesto por la columna y la fila anteriores
   int filaSiguiente = tablaDeTransicion[*filaAnterior][columnaAnterior];
   *filaAnterior = filaSiguiente;
}

/*int esPalabra (const char *s){
	
	int e;
	unsigned int i;
	
	for (e=0,i=0; s[i]!='\0';i++){
		
		e = tablaDeTransicion [e][columna(s[i])];
	}
	
	return e;
}*/


int determinarColumna(char letra){
   int columnaADevolver = 0;
   switch(letra)
   {
    case '-': //Caso -
	case '+': // Caso +
		columnaADevolver = 0;
      break;
	case '0': // Caso de 0
      columnaADevolver = 1;
      break;
   	case 49 ... 55: // Caso de 1-7
      	columnaADevolver = 2;
      //  fila = tablaDeTransicion[fila][1]
      break;
   	case 56 ... 57: // Caso de 8-9
      	columnaADevolver = 3;
      break;
   	case 65 ... 70: // Caso de A-F (mayuscula)
   	case 97 ... 102: // Caso de a-f (minuscula)
      	columnaADevolver = 4;
      break;
   	case 'X': // Caso x (minuscula)
   	case 'x': // Caso X (mayuscula)
      	columnaADevolver = 5;
      break;
   default: // Caso de Error, si no devuelve ninguno de los casos anteriores: {no rec} en tabla
      columnaADevolver = 6;
      break;
   }
   return columnaADevolver;
}


int procesarPalabra(char palabraRecibida[], int largoPalabra){
   int tipoDeConstante = 0; // si no se reconoce quedarÃ¡ en 0
   int columnaActual = 0,i = 0;
   int * filaActual;
   int h=0;
   filaActual= &h;
  
   
   //procesa letra por letra y va actualizando los estados
      while(palabraRecibida[i]!= '\0' || filaActual == 7){
         columnaActual=determinarColumna(palabraRecibida[i]);
         actualizarEstado(columnaActual,filaActual);

         i++;
      }

   if(tablaDeTransicion[*filaActual][columnaActual]!=7 ){  //si el valor del estado final es 6 sabemos que no es valida

      switch(*filaActual)
      {
      case 2:// Caso de Octal
      case 6: 
         tipoDeConstante = 1;
         break;

      case 3: // Caso de Decimal
         tipoDeConstante = 2;
         break;

      case 5: // Caso de Hexadecimal
         tipoDeConstante = 3;
         break;
      }
   }
   return tipoDeConstante;
}