#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int procesarPalaba (char palabraRecibida[], int largoPalabra);
void actualizarEstado (int columnaAnterior, int *filaAnterior);
int determinarColumna (char letra);
void grabarPalabra (char palabraAnalizada[], int tipoDeConstate, FILE *fSalida);

int tablaDeTransicion[4][4] = {
    {1, 3, 3, 3},
    {1, 2, 1, 3},
    {1, 3, 3, 3},
    {3, 3, 3, 3},
};

/*01239123&0&01231231239&1231231232321312r&z467&-123&4A67&A467&AAAA&4a67&01234567&ZZZZZZ&86-0,2;4/&+1234123&b444&10&-123&0x123&0xAAA&0xf5f&22
*/
int main(){
   //se abre el archivo de lectura
   FILE *fEntrada = fopen("Entrada.txt", "r");// o la posicion de disco duro en donde lo tengan guardado
   //se abre el archivo de escritura
   FILE *fSalida = fopen("Salida.txt", "w");

    char letra;
    int j = 0, i = 0, k = 0;
    char palabraAux[150]={""};
   /* fprintf(fSalida,"palabraAux:  %s",palabraAux);*/
 /*  fprintf(fSalida,"Mayus:  %d",'A');
   fprintf(fSalida,"Minus:  %d",'a');*/
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
    if (tipoDeConstate == 0) {
      fprintf (fSalida, "%s \t\t %s\n", palabraAnalizada, "Es discapacitado"); 
    }
    else if (tipoDeConstate == 1){
        int resultado = sacarCalculo(palabraAnalizada)
      fprintf (fSalida, "%s \t\t %s\n", palabraAnalizada, "Es valido");
    }
    
}

int sacarCalculo(char palabraAnalizada[]){
    
}


void actualizarEstado(int columnaAnterior, int *filaAnterior){ //el estado anterior deberia estar compuesto por la columna y la fila anteriores
   int filaSiguiente = tablaDeTransicion[*filaAnterior][columnaAnterior];
   *filaAnterior = filaSiguiente;
}




int determinarColumna(char letra){
   int columnaADevolver = 0;
   switch(letra)
   {
    case '-': //Caso -
        *menos = true;
        columnaADevolver = 1;
        break;
    case '+': // Caso +
        *mas = true;
        columnaADevolver = 1;
        break;
    case '*': // caso *
        *por = true;
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
}


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