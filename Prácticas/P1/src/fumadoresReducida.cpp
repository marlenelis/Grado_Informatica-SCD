// *****************************************************************************
//
// Prácticas de SCD. Práctica 1.
// Ejercicio de los fumadores simplificada.
//
// Autor:			Marlene E. Vásquez
// Compilar: 		g++ -o fumadores fumadores_2.cpp -lpthread
// Uso: 			./fumadores
//
// *****************************************************************************
#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

using namespace std ;
// ---------------------------------------------------------------------
const unsigned long nFumadores = 3;
sem_t	s_estanquero,
		sem_fumadores[nFumadores];
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

// ----------------------------------------------------------------------------
// función que simula la acción de fumar, como un retardo aleatorio de la hebra.
// recibe como parámetro el numero de fumador
// el tiempo que tarda en fumar está entre dos y ocho décimas de segundo.

void fumar( int num_fumador )
{
   cout << "\tFumador número " << num_fumador << ": comienza a fumar." << endl << flush ;
   retraso_aleatorio( 0.2, 0.8 );
   cout << "\tFumador número " << num_fumador << ": termina de fumar." << endl << flush ;
}
// ----------------------------------------------------------------------------
// funcion estanquero: selecciona aleatoriamente un ingrediente de los tres que se necesita
// para fumar un cigarro (cerillas, tabaco, papel). Desbloquea al fumador que necesita dicho ingrediente y despues
// se bloquea esperando la retirada del ingrediente.
// Esta función es usada por la hebra hebra_estanquero.
void * estanquero(void *){
	int ingrediente;
	while(true){
		sem_wait(&s_estanquero);//se bloquea el estanquero
		ingrediente = rand()%3; // selecciona el ingrediente aleatoriamente.
		switch(ingrediente){ 
			case 0: cout<< "MOSTRADOR: CERILLAS " <<endl<<flush;  break;
			case 1: cout<< "MOSTRADOR: TABACO   " <<endl<<flush;  break;
			case 2: cout<< "MOSTRADOR: PAPEL    " <<endl<<flush;  break;
		}
		sem_post(&sem_fumadores[ingrediente]);// desbloquea al fumador que necesita el ingrediente seleccionado.
	}
}
//-----------------------------------------------------------------------------
// Funcion fumador: Esta funcion recibe como parametro el identificador del fumador.
// El fumador desbloqueado toma el ingrediente del mostrador, desbloquea al estanquero y 
// despues fuma (realiza una llamada a la función fumar()).
// Esta funcion es usada por la hebra fumadores[i].
void * fumador(void * id){
	unsigned long id_fumador= (unsigned long) id;
	while(true){	
		sem_wait(&sem_fumadores[id_fumador]);
		switch(id_fumador){ // el fumador desbloaqueado toma el ingrediente del mostrador
			case 0:	cout<<"\tFumador número 1: \033[1;31mrecogiendo cerillas.\033[0m" <<endl<<flush; 	break;
			case 1: cout<<"\tFumador número 2: \033[1;33mrecogiendo tabaco.\033[0m"	  <<endl<<flush; 	break;
			case 2: cout<<"\tFumador número 3: \033[1;36mrecogiendo papel.\033[0m" 	  <<endl<<flush;	break;
		}						
		sem_post(&s_estanquero);//desbloquea al estanquero
		fumar(id_fumador+1);//fuma
	}
}
// ----------------------------------------------------------------------------
int main()
{
  	srand( time(NULL) ); // inicializa semilla aleatoria para selección aleatoria de fumador
	//declaración de las hebras
	pthread_t 	hebra_estanquero ,
			fumadores[nFumadores];
	//inicialización de los semáforos
	sem_init(&s_estanquero,0,1);
	for(int i=0 ;i<nFumadores;i++)
		sem_init(&sem_fumadores[i],0,0);

	//creación de las hebras
	pthread_create(&hebra_estanquero, NULL, estanquero, NULL);
	for( unsigned long i = 0; i< nFumadores; i++)
		pthread_create(&fumadores[i],  NULL, fumador,	(void*)i );	
	
	//espera de las hebras
	pthread_join(hebra_estanquero,NULL);
	for( unsigned long i = 0; i< nFumadores; i++)
		pthread_join(fumadores[i],  NULL );	
	
	//destruir semaforos y liberación de memoria ocupada
	sem_destroy(&s_estanquero);
	for(int i=0 ;i<nFumadores;i++)
		sem_destroy(&sem_fumadores[i]);

  	return 0 ;
}
