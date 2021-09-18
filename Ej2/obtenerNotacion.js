const pushearOperador = (operadores, stack) => {
  [...operadores].forEach(operador => stack.push(operador))
}

const obtenerNotacion = (palabraAnalizada) => {
  let i = 0;
  let stack = []
  let notacion = "";
  let numeroActual = "";
  let operadorEnEspera = ""; 
  // analizo la operación que ya sé que es válida
  while(palabraAnalizada.length > i){
        //no es un operador, es un numero y obtengo todos sus digitos;
        if(palabraAnalizada[i] != '+' && palabraAnalizada[i] != '*' && palabraAnalizada[i] != '-'){
            numeroActual = numeroActual + palabraAnalizada[i]
            i = i +1;
        }
        //es un operador
        else{
          //como es un operador, el numero que lo precede ya es parte de la notacion
          notacion = notacion + numeroActual;
          stack.push(numeroActual)
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
                pushearOperador(operadorEnEspera, stack)
                operadorEnEspera = palabraAnalizada[i];
              }
          }
          //el operador es de mayor prioridad, el caracter vacio o
          //un conjunto de operadores
          else{
             notacion = notacion  + operadorEnEspera;
             pushearOperador(operadorEnEspera, stack)
              operadorEnEspera = palabraAnalizada[i]
          }
          i = i + 1;
        }
  }
  //guardo lo que quedó en las variables al terminar el loop
  stack.push(numeroActual)
  pushearOperador(operadorEnEspera, stack)

  return stack;
}

const operacion = "41-42+53-5-2-5+53*24+24+532-142";
const operacion2 = "41-42+53-5-2-5+53*24+24+532-142+25+23+235+23+523+52+355*235*23-5*23-523-523-523523-52-5*23"
const notacion = obtenerNotacion(operacion)
const notacion2 = obtenerNotacion(operacion2)

//para correr el codigo: https://onecompiler.com/javascript

// para corroborar las notaciones: https://www.dcode.fr/reverse-polish-notation

const evaluarNotacion = tokens => {
  const operands = ['+', '-', '*'];
  const stack = [];

  for (let s of tokens) {
    if (operands.indexOf(s) >= 0) {
      const n2 = parseInt(stack.pop());
      const n1 = parseInt(stack.pop());
      stack.push(evaluar(n1, n2, s));
    } else {
      stack.push(s);
    }
  }

  return parseInt(stack.pop());
};

const evaluar = (n1, n2, operador) => {
  if (operador === '+') return n1 + n2;
  if (operador === '-') return n1 - n2;
  if (operador === '*') return n1 * n2;
};


const resultado = evaluarNotacion(notacion)
const resultado2 = evaluarNotacion(notacion2)

console.log(notacion2)
console.log(resultado, resultado2, 'aca')