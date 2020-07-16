# TSSOO-taller02

##### Fernando Del Pino Machuca - fernando.delpino@alumnos.uv.cl

###### Universidad de Valparaíso

---


## 1. Introducción

El siguiente repositorio se basa en el desarrollo de la actividad del taller 2 de la asignatura taller de sistemas operativos. El objetivo y proposito de este, no es mas que la utilizacion y manejo de Threads (Hilos) para almacenar y sumar datos de un arreglo de numeros aleatorios.
La resolucion de este taller esta basado en el lenguaje de programación [C++](https://es.wikipedia.org/wiki/C%2B%2B) y ademas, todo el proceso resolutivo y de pruebas se ejecuto en una maquiuna virtual con sistema operativo Ubuntu-server.

## 2. Diseño de Solución

Este Taller presenta 2 modulos, un modulo de llenado de arreglo, y otro modulo donde se realizará la suma del arreglo anterior, todo esto se realizará mediante un proceso de rellenado en paralelo y en serie.

El orden y la estructura de la solucion propuesta se basa a grandes rasgos en el siguiente diseño.

![Imgur](https://imgur.com/bj5IL0J)

Al momento de ingresar los argumentos se debe especificar el tamaño de elementos que poseera el arreglo, luego se debe incluir la cantidad de hilos que se desean crear para la ejecucion del programa, y finalmente se incluiran los limites inferiores y superiores que funcionaran como rango de numeros aleatorios con los que se rellenaran los arreglos. Tras este proceso se realiza el proceso de creacion de los arreglos, el arreglo serial y uno paralelo para la comparacion de los resultados de desempeños. Una vez creado el arreglo de numeros aleatorios serial, se traspasarán los elementos hacia un arreglo que sera utilizado por los hilos de forma paralela, para luego entregar un solo arreglo que sera dividido nuevamente por otros hilos para lograr la suma de estos elementos. 

## 3. Estructura del Código

### 3.1 Chequeo de argumentos

Para comenzar se debe destacar que el archivo "checkArgs.hpp" se encargara de traspasar los argumentos que se ingresen al programa para ser utilizados como variables por este mismo. Su comportamiento y analisis es simple.

### 3.2 Modulo 1

Lo primero es crear el arreglo serial y paralelo, donde el arreglo serial se rellenará de la siguiente manera:
```
    std::uniform_int_distribution<uint32_t> unif(LInferior, LSuperior);
    for(uint64_t i = 0; i < totalElementos; i++){
        ArregloComun[i] = unif(rng);
    }
```
Lo que se realizara sera crear un generador de numeros aleatorios, del tipo entero sin signo de 32 bits, seteando a "LInferior" y "LSuperior" como los limites de los numeros aleatorios. Luego tendremos un bucle que recorra el arreglo comun, almacenando el numero aleatorio dentro de cada posicion del total de elementos que poseera el arreglo.

Paso seguido se instancio que se crearan 2 arreglos, otro serial, y uno para los hilos, es decir un "ArregloSerial" y "ArregloParalelo", donde en las siguientes lineas de codigo se instanciaran y llamaran a la funcion que correspondera al llenado de estos arreglos.

```
void LLenadoArreglo(size_t Left, size_t Right, size_t RandType){
    for(size_t i = Left; i < Right; ++i){       
        switch(RandType){
            case 0: 
                ArregloSerial[i] = ArregloComun[i]; 
                break;
            case 1:
                ArregloParalelo[i] = ArregloComun[i];
                break;
        }
    }
}
```
Lo que realizara esta funcion de buenas a primeras es recibir los parametros con que fue llamada, parametros que se utilizaran para escoger en que punto del arreglo se guardaran los numeros aleatorios del "ArregloComun". Se analizará si el llenado corresponde al arreglo paralelo o al arreglo serial mediante la utilizacion de casos.

### 3.2 Modulo 2.

Este modulo fue un poco mas complejo, ya que, por un lado, el arreglo paralelo realizo la suma de sus elementos a traves de otra funcion, mientras que la suma del arreglo serial no.

```
    for (size_t i=0;i<totalElementos;i++){
        SumaSerial += ArregloSerial[i];
    }
```
La Suma serial se realizo mediante una variable que recorriera el arreglo serial en un bucle. Esto no nos dio problemas de incosistencia de datos como en el caso de la suma en paralelo, por lo que debido a temas de tiempo y complejidad se abarco esta solucion simplemente mediante el uso de exclusion mutua

```
void SumadoParalelo(uint64_t Left, uint64_t Right){
    Candado.lock();
    for(uint64_t i = Left; i < Right; i++){
        SumaParalelo += ArregloParalelo[i];
    }
    Candado.unlock();
}
```
Se entiende que posiblemente esto nos entrego mayores tiempos de ejecucion del modulo de sumado por parte de los Threads, pero por temas de tiempo, errores y complejidad, se estimo que era una solucion factible.

## 4. Conclusión.

Todo este programa esta basado en su implementacion dentro de una maquiuna virtual con sistema operativo basado en Linux, es muy posible que ciertas caracteristicas no sean del todo correctas para otros sistemas operativos y ciertas funcionalidades del programa no sean eficazmente logradas.

Como observacion, no se busco implementar el mejor desempeño para la utilizacion de hilos, pero se entendia que algunas estructuras pudieron escribirse de distinta manera quizas mejor implementadas como el apartado de la suma pudo ser mucho mas conveniente la utilizacion de un vector que guardara los datos para luego consolidar los datos de otra manera, lo que seria mucho más optímo, correspondientemente claro, a un mejor manejo de los hilos y arreglos.

Se puede apreciar que el cumplimiento del taller no abarco por completo ciertos aspectos:

```diff
- Optimización de los tiempos de procesamiento por parte de los hilos
- Cierto entendimiento de estructuras de vectores para el manejo de los hilos
- Entendimiento de como realizar un llenado de arreglos por parte del hilo de manera mas optima
```

Aunque tambien es cierto que el proceso de aprendizaje fue logrado para la completacion de los siguientes aspectos:

```diff
+ implementación del módulo 1
+ implementación del módulo 2
+ Cierto grado del manejo de tareas por parte de Threads
+ Mayor comprension del funcionamiento y desarrollo de programas en lenguaje C++
+ Entendimiento del comportamiento y utilizacion de la memoria fisica y virtual de un equipo computacional
```

