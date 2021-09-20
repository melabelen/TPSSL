#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int procesarPalaba (char palabraRecibida[], int largoPalabra, int* octal, int* hexa, int* decimal);
void actualizarEstado (int columnaAnterior, int *filaAnterior);
int determinarColumna (char caracter);
void grabarPalabra (char palabraAnalizada[], int tipoDeConstate, FILE *fSalida);

int tablaDeTransicion[8][7] = {
      /* q0  */{ 1, 2, 3, 3, 7, 7, 7},
      /* q1+ */{ 7, 7, 3, 3, 7, 7, 7},
      /* q2+ */{ 7, 6, 6, 7, 7, 4, 7},  
      /* q3  */{ 7, 3, 3, 3, 7, 7, 7},  
      /* q4+ */{ 7, 5, 5, 5, 5, 7, 7}, 
      /* q5+ */{ 7, 5, 5, 5, 5, 7, 7},  
      /* q6+ */{ 7, 6, 6, 7, 7, 7, 7},  
      /* q7	 */{ 7, 7, 7, 7, 7, 7, 7},  
};

/*01239123&0&01231231239&1231231232321312r&z467&-123&4A67&A467&AAAA&4a67&01234567&ZZZZZZ&86-0,2;4/&+1234123&b444&10&-123&0x123&0xAAA&0xf5f&22
*/
int main(){
    //Abro el archivo de lectura
    FILE *fEntrada = fopen("Entrada.txt", "r");
    //Abro el archivo de escritura
    FILE *fSalida = fopen("Salida.txt", "w");

    char caracterActual;
    int j = 0, i = 0, k = 0;
    int decimal = 0, hexa=0,octal=0;
    char numeroActual[150]={""};
    while(!feof(fEntrada)){
        //Leo un caracter del archivo
        caracterActual = fgetc(fEntrada);
        //Si sigo procesando el mismo supuesto numero, lo guardo caracter a caracter
        if (caracterActual != '&' && !feof(fEntrada)){
              numeroActual[i] = caracterActual;
              i++;
        }
        else{ //Termina el numero
                 //proceso el numeroActual
                char Tipo=procesarPalabra(numeroActual,i, &octal, &hexa, &decimal);

                //Grabo la palabra en el archivo de salida
                grabarPalabra(numeroActual,Tipo,fSalida);

                //Se reincia la palabra
                for (k = 0; k < i; k++){
                  numeroActual[k] = '\0';
                }
                //Se reinicia el contador
                i=0;
                
        }
      } 
      
    fprintf(fSalida, "Hubo %d hexadecimales, %d octales y %d decimales", hexa, octal, decimal);
    //Cierro los archivos
    fclose(fEntrada);
    fclose(fSalida);
    printf("%s", "Procesamiento Terminado");
    return 0;
}


void grabarPalabra (char palabraAnalizada[], int tipoDeConstate, FILE *fSalida){
     //Escribo al final del archivo de salida el numero y su tipo
    fseek(fSalida, 0, SEEK_END);
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


void actualizarEstado(int columnaAnterior, int *filaAnterior){ 
    //Busco cual es la fila (o estado) siguiente en la tabla a partir de la fila y columna anteriores
   int filaSiguiente = tablaDeTransicion[*filaAnterior][columnaAnterior];
   //Actualizo el estado
   *filaAnterior = filaSiguiente;
}




int determinarColumna(char caracter){
   int columnaADevolver = 0;
   switch(caracter)
   {
    case '-': 
	case '+':
		columnaADevolver = 0;
      break;
	case '0': 
      columnaADevolver = 1;
      break;
    // ASCII para los numeros 1 a 7
   	case 49 ... 55: 
      	columnaADevolver = 2;
      break;
    // ASCII para los numeros 8 y 9
   	case 56 ... 57: 
      	columnaADevolver = 3;
      break;
    // ASCII para las letras A-F y a-f
   	case 65 ... 70: 
   	case 97 ... 102: 
      	columnaADevolver = 4;
      break;
   	case 'X': 
   	case 'x':
      	columnaADevolver = 5;
      break;
    //Si no reconoce ninguno de los anteriores caracteres, va al estado de error
   default: 
      columnaADevolver = 6;
      break;
   }
   return columnaADevolver;
}


int procesarPalabra(char palabraRecibida[], int largoPalabra, int* octal, int* hexa, int*decimal ){
   int tipoDeConstante = 0;
   int columnaActual = 0,i = 0;
   int * filaActual;
   int h=0;
   filaActual= &h;
  
   
   //Caracter a caracter, recorre el automata a partir de la tabla
      while(palabraRecibida[i]!= '\0'){
         columnaActual=determinarColumna(palabraRecibida[i]);
         actualizarEstado(columnaActual,filaActual);

         i++;
      }
        
    //Dependiendo del estado final en el que queda, determina el tipo de la constante
      switch(*filaActual)
      {
      case 2:// Caso de Octal
      case 6: 
         tipoDeConstante = 1;
         *octal = *octal + 1;
         break;

      case 3: // Caso de Decimal
         tipoDeConstante = 2;
         *decimal = *decimal + 1;
         break;

      case 5: // Caso de Hexadecimal
         tipoDeConstante = 3;
         *hexa = *hexa +1;
         break;
         
     case 7:
        tipoDeConstante = 0;
        break;
      }
   
   return tipoDeConstante;
}