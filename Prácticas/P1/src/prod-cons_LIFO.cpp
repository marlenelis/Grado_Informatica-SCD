// *****************************************************************************
//
// Prácticas de SCD. Práctica 1.
// Ejercicio del productor-consumidor con buffer intermedio gestionado con la alternativa LIFO(Pila acotada)
//
// Autor:			Marlene E. Vásquez
// Compilar: 		g++ -o prod-cons prod-cons_LIFO.cpp -lpthread
// Uso: 			./prod-cons
//
// *****************************************************************************

#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // necesario para {\ttbf usleep()}
#include <stdlib.h> // necesario para {\ttbf random()}, {\ttbf srandom()}
#include <time.h>   // necesario para {\ttbf time()}

using namespace std ;

// ---------------------------------------------------------------------
// constantes configurables:

const unsigned
  num_items  = 40 ,    // numero total de items que se producen o consumen
  tam_vector = 10 ;    // tamaño del vector, debe ser menor que el número de items

//declaracion de los semaforos
sem_t 	puede_escribir,
		puede_leer,
		mutex;

int buffer[tam_vector];//buffer
int primera_libre;//variable para la gestion de ocupación, esta variable se incrementa al escribir
		  // y se decrementa al leer. (LIFO)

// ---------------------------------------------------------------------
// introduce un retraso aleatorio de duración comprendida entre
// 'smin' y 'smax' (dados en segundos)

void retraso_aleatorio( const float smin, const float smax )
{
  static bool primera = true ;
  if ( primera )        // si es la primera vez:
  {  srand(time(NULL)); //   inicializar la semilla del generador
     primera = false ;  //   no repetir la inicialización
  }
  // calcular un número de segundos aleatorio, entre {\ttbf smin} y {\ttbf smax}
  const float tsec = smin+(smax-smin)*((float)random()/(float)RAND_MAX);
  // dormir la hebra (los segundos se pasan a microsegundos, multiplicándos por 1 millón)
  usleep( (useconds_t) (tsec*1000000.0)  );
}

// ---------------------------------------------------------------------
// función que simula la producción de un dato

unsigned producir_dato()
{
  static int contador = 0 ;
  contador = contador + 1 ;
  retraso_aleatorio( 0.1, 0.5 );
  cout << "Productor : dato producido: " << contador << endl << flush ;
  return contador ;
}
// ---------------------------------------------------------------------
// función que simula la consumición de un dato

void consumir_dato( int dato )
{
   retraso_aleatorio( 0.1, 1.5 );
   cout << "Consumidor: \t\t\tdato consumido: " << dato << endl << flush ;
}
// ---------------------------------------------------------------------
// función que ejecuta la hebra del productor

void * funcion_productor( void * )
{
  for( unsigned i = 0 ; i < num_items ; i++ )
  {
    int dato = producir_dato() ;
	sem_wait(&puede_escribir);		
		sem_wait(&mutex);
	    	//insertar "dato" en el vector o buffer
			buffer[primera_libre]= dato;		
			primera_libre++; 
			 cout << "Productor : \033[1;33mdato insertado: \033[0m" << dato << endl << flush ;
		sem_post(&mutex);  	
	sem_post(&puede_leer);
  }
  return NULL ;
}
// ---------------------------------------------------------------------
// función que ejecuta la hebra del consumidor

void * funcion_consumidor( void * )
{
  for( unsigned i = 0 ; i < num_items ; i++ )
  {
    int dato ;
	sem_wait(&puede_leer);		
		sem_wait(&mutex);
	    	//leer "dato" desde el vector intermedio
	   		primera_libre--;
			dato = buffer[primera_libre];
	    	cout << "Consumidor:\t\t\t\033[1;31mdato extraído : \033[0m" << dato << endl << flush ;	
		sem_post(&mutex);
	sem_post(&puede_escribir);
    consumir_dato( dato ) ;
  }
  return NULL ;
}
//----------------------------------------------------------------------

int main()
{
	//declaracion de las hebras.	
	pthread_t 	hebra_escritora,
				hebra_lectora;
	//inicio de los semaforos
	sem_init(&puede_escribir,0,tam_vector);
	sem_init(&puede_leer,	 0,0);
	sem_init(&mutex,0,1);
	//creación de las hebras
	pthread_create(&hebra_escritora,NULL, funcion_productor, NULL);
	pthread_create(&hebra_lectora, 	NULL, funcion_consumidor,NULL);
	//espera de las hebras
	pthread_join(hebra_escritora,  NULL);
	pthread_join(hebra_lectora,    NULL);
	cout<< "FIN"<<endl;
	//destruye los semaforos y libera la memoria ocupada
	sem_destroy(&puede_escribir);
	sem_destroy(&puede_leer);
	sem_destroy(&mutex);
 

   return 0 ;
}
