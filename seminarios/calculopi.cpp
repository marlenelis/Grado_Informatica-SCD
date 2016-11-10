// *********************************************************************
// SCD. Ejemplos del seminario 1.
//
// cálculo de PI
// *********************************************************************
/* 
 * Author: Marlene E. Vásquez
 *
 */
#include <iostream>
#include <pthread.h>
#include "fun_tiempo.h"

using namespace std ;

// ---------------------------------------------------------------------
// constante y variables globales (compartidas entre hebras)

const unsigned long m = long(1024)*long(1024) ;  // número de muestras (un millón)
const unsigned n      = 4 ;                      // número de hebras

double resultado_parcial[n] ; // tabla de sumas parciales (una por hebra)

// ---------------------------------------------------------------------
// implementa función $f$

double f( double x )       
{ 
   return 4.0/(1+x*x) ;     // $~~~~f(x)\,=\,4/(1+x^2)$
}
// ---------------------------------------------------------------------
// cálculo secuencial

double calcular_integral_secuencial( )
{  
   double suma = 0.0 ;                      // inicializar suma
   for( unsigned long i = 0 ; i < m ; i++ ) // para cada $i$ entre $0$ y $m-1$
      suma += f( (i+0.5)/m );               // $~~~~~$ añadir $f(x_i)$ a la suma actual
   return suma/m ;                          // devolver valor promedio de $f$
}
// ---------------------------------------------------------------------
// función que ejecuta cada hebra

void * funcion_hebra( void * ih_void ) 
{  
   unsigned long ih = (unsigned long) ih_void ; // número o índice de esta hebra
   double sumap = 0.0 ;
   // calcular suma parcial en "sumap"
   //.....
   unsigned long inicio,fin, muestras;
   muestras = (m/n);//tamaño del intervalo
   inicio = muestras*ih; // inicio del intervalo - de cada hebra
   fin = (inicio)+(muestras);//fin del intervalo - de cada hebra

   for(unsigned long i = inicio; i< fin ; i++ ) // para cada $i$ entre inicio y fin
      sumap += f( (i+0.5)/m );                  // añadir f(xi) a la suma actual
   
   resultado_parcial[ih] = sumap; // guardar suma parcial en vector.
   return NULL ;
}
// ---------------------------------------------------------------------
// cálculo concurrente

double calcular_integral_concurrente( )
{  
   double resultado=0.0;
   pthread_t hebras[n];    

   // crear y lanzar $n$ hebras, cada una ejecuta "funcion\_concurrente"
   //.....
   for(unsigned long i=0; i<n ;i++)      
      pthread_create( &hebras[i], NULL,funcion_hebra, (void *)i);
   
   // esperar (join) a que termine cada hebra, sumar su resultado
   //.....
   for(unsigned long i=0; i<n ;i++)
      pthread_join(hebras[i], NULL);     
   
   // devolver resultado completo
   // .....
   for(unsigned long i=0; i<n ;i++)     
      resultado+= resultado_parcial[i]; //sumar el resultado parcial de todas las hebras
   
   return resultado/m; // Devolver el promedio 
}

// ---------------------------------------------------------------------

int main()
{
   cout << "Ejemplo 4 (cálculo de PI)" << endl ;
   double pi_sec = 0.0, pi_conc = 0.0 ;
   //=======================================================
   //               SECUENCIAL
   //=======================================================
   const struct timespec  ini_sec= ahora();
   pi_sec  = calcular_integral_secuencial() ;   
   const struct timespec  fin_sec= ahora();
  
   cout  <<"\n" << "tiempo transcurrido == " // escribe resultados:
         << duracion( &ini_sec, &fin_sec ) // tiempo en segundos entre ”inicio” y ”fin”
         << " seg." << endl ;
   
   cout << "valor de PI (calculado secuencialmente)  == " << pi_sec  << endl ;
   ///=======================================================
   //               CONCURRENTE
   //=======================================================
   const struct timespec ini_conc = ahora();            
   pi_conc = calcular_integral_concurrente() ;
   const struct timespec fin_conc = ahora();
   
   cout  << "\ntiempo transcurrido == " // escribe resultados:
         << duracion( &ini_conc, &fin_conc ) // tiempo en segundos entre ”inicio” y ”fin”
         << " seg." << endl ;
    
   cout << "valor de PI (calculado concurrentemente) == " << pi_conc << endl ; 
    
   return 0 ;

}
// ---------------------------------------------------------------------



// ----------------------------------------------------------------------------
