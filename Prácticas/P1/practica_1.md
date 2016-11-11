###Práctica 1: Sincronización de hebras con semáforos
-----

**Objetivos:** realizar dos implementaciones de dos problemas sencillos de sincronización.

1. **Problema: Productor Consumidor**
2. **Problema: Fumadores**
- [Descripción de los problemas][descripcion]

### Problema del productor-consumidor

- [***Solucion 1:***][LIFO] Pila Acotada(LIFO)
- [***Solución 2:***][FIFO] Cola Circular(FIFO)

### Problema de los Fumadores

- [***Solucion 1:***][ampliada] Una función por cada fumador.
- [***Solución 2:***][reducida] simplificación de código.


### Problema extra

<img src="problema.JPG" alt="Logotipo" width="70%" height="70%">

- [***Solución:***][suma] Suma de los elementos pares e impares.



### Evaluación
- [***Ejercicio 1:***][ex1] Suma de los elementos pares e impares.

```sh
 Nota: este problema es similar al problema extra pero con la modificacion en el P0, primero rellena los datos del vector y luego despierta a P1 y a P2 y espera a que terminen para realizar la suma total con los resultados obtenidos (total = par + impar;).
```
[P0]
rellenar datos;
despierta P1;
despierta P2;
espera P1;//pares
espera P2;//impares
escribe suma total

[P1]
espera a P0;
suma solo elementos pares;
escribe resultado;

[P2]
espera a P0;
suma los elemntos impares
escribe el resultado;


- [***Ejercicio 2:***][ex2] modificación Productor consumidor

**Problema**
- producir datos entre 10 y 30;

- crear 2 hebras consumidoras 
 - 1 hebra consume datos pares
 - 2 hebra consume datos impares
```sh
Tener en cuenta la finalización ya que no se sabe cuantos elemetos pares e impares va ha generar el productor.
```


[descripcion]:https://github.com/marlenelis/SCD/blob/master/Prácticas/scd-practica1.pdf



[LIFO]:https://github.com/marlenelis/SCD/blob/master/Prácticas/P1/src/prod-cons_LIFO.cpp
[FIFO]:https://github.com/marlenelis/SCD/blob/master/Prácticas/P1/src/prod-cons_FIFO.cpp

[ampliada]:https://github.com/marlenelis/SCD/blob/master/Prácticas/P1/src/fumadores.cpp
[reducida]:https://github.com/marlenelis/SCD/blob/master/Prácticas/P1/src/fumadoresReducida.cpp


[suma]:https://github.com/marlenelis/SCD/blob/master/Prácticas/P1/src/suma_vector.cpp
[ex1]:https://github.com/marlenelis/SCD/blob/master/Prácticas/P1/src/ex-suma_vector.cpp
[ex2]:https://github.com/marlenelis/SCD/blob/master/Prácticas/P1/src/ex-prod-cons.cpp


