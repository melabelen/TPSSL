const obtenerNotacion = (palabraAnalizada) => {
    let i = 0;
    let notacion = "";
    let numeroActual = "";
    let operadorEnEspera = ""; 
    // analizo la operación que ya sé que es válida
    while(palabraAnalizada.length > i){
          console.log(palabraAnalizada[i])
          //no es un operador, es un numero y obtengo todos sus digitos;
          if(palabraAnalizada[i] != '+' && palabraAnalizada[i] != '*' && palabraAnalizada[i] != '-'){
              numeroActual = numeroActual + palabraAnalizada[i]
              i = i +1;
          }
          //es un operador
          else{
            //como es un operador, el numero que lo precede ya es parte de la notacion
            notacion = notacion + numeroActual;
            //reseteo el numeroActual
            numeroActual = "";
            //El operador es de los de menor prioridad
            if(operadorEnEspera == "+" || operadorEnEspera == '-'){
              //verifico si el operador actual tiene mayor prioridad
                if(palabraAnalizada[i] == '*'){
                  //la notacion toma al operador de mayor prioridad y el de menor
                  //sigue en espera
                  operadorEnEspera = '*'+operadorEnEspera
                }
                else{
                  //el operador en espera pasa a ser parte de la notacion 
                  //el operador analizado actualmente pasa a estar en espera
                  notacion = notacion + operadorEnEspera;
                  operadorEnEspera = palabraAnalizada[i];
                }
            }
            //el operador es de mayor prioridad, el caracter vacio o
            //un conjunto de operadores
            else{
               notacion = notacion  + operadorEnEspera;
                operadorEnEspera = palabraAnalizada[i]
            }
            i = i + 1;
          }
    }
    //guardo lo que quedó en las variables al terminar el loop
    notacion = notacion + numeroActual + operadorEnEspera
    
    return notacion;
    
  }
  
  console.log(obtenerNotacion("3*2+5*3-2*6"))


  //para correr el codigo: https://onecompiler.com/javascript

  // para corroborar las notaciones: https://www.dcode.fr/reverse-polish-notation