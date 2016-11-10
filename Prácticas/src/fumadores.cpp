// *****************************************************************************
//
// Prácticas de SCD. Práctica 1.
// Ejercicio de los fumadores, una funcion por cada fumador.
//
// Autor:			Marlene E. Vásquez
// Compilar: 		g++ -o fumadores fumadores.cpp -lpthread
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
//declaracion de los semaforos
sem_t	s_estanquero,
	sem_fumador_1,
	sem_fumador_2,
	sem_fumador_3;
//declaracion de las hebras
pthread_t 	hebra_estanquero ,
			hebra_fumador_1,
			hebra_fumador_2,
			hebra_fumador_3;
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
		sem_wait(&s_estanquero);
		ingrediente = rand()%3;
		switch(ingrediente){
			case 0: cout<< "MOSTRADOR: CERILLAS "<<endl<<flush; 	sem_post(&sem_fumador_1); break;
			case 1: cout<< "MOSTRADOR: TABACO   "<<endl<<flush; 	sem_post(&sem_fumador_2); break;
			case 2: cout<< "MOSTRADOR: PAPEL    "<<endl<<flush; 	sem_post(&sem_fumador_3); break;
		}
	}
}
// funcion que simula al fumador 1, el ingrediente que necesita para fumar son las cerillas 
void * fumador_1(void *){//espera a cerillas
	while(true){		
		sem_wait(&sem_fumador_1);
				cout<<"\tFumador número 1: \033[1;31mrecogiendo cerillas.\033[0m"	<<endl<<flush;
		sem_post(&s_estanquero);
		fumar(1);
	}
}
// funcion que simula al fumador 2, el ingrediente que necesita para fumar es el tabaco
void * fumador_2(void *){ //espera a tabaco
	while(true){		
		sem_wait(&sem_fumador_2);
			 	cout<<"\tFumador número 2: \033[1;33mrecogiendo tabaco.\033[0m"		<<endl<<flush;
		sem_post(&s_estanquero);
		fumar(2);
	}
}
// funcion que simula al fumador 3, el ingrediente que necesita para fumar es el papel
void * fumador_3(void *){//espera a papel
	while(true){		
		sem_wait(&sem_fumador_3);
				cout<<"\tFumador número 3: \033[1;36mrecogiendo papel.\033[0m"		<<endl<<flush;
		sem_post(&s_estanquero);
		fumar(3);
	}
}
// ----------------------------------------------------------------------------

int main()
{
  	srand( time(NULL) ); // inicializa semilla aleatoria para selección aleatoria de fumador
		
	//inicailización de los semáforos
	sem_init(&s_estanquero, 0,1);// iniciado a 1, para que primero se genere el ingrediente.
	sem_init(&sem_fumador_1,0,0);
	sem_init(&sem_fumador_2,0,0);
	sem_init(&sem_fumador_3,0,0);
	//creación de las hebras
	pthread_create(&hebra_estanquero, NULL, estanquero, NULL);
	pthread_create(&hebra_fumador_1,  NULL, fumador_1 , NULL);
	pthread_create(&hebra_fumador_2,  NULL, fumador_2 , NULL);
	pthread_create(&hebra_fumador_3,  NULL, fumador_3 , NULL);
	//espera de las hebras
	pthread_join(hebra_estanquero,NULL);
	pthread_join(hebra_fumador_1, NULL);
	pthread_join(hebra_fumador_2, NULL);
	pthread_join(hebra_fumador_3, NULL);
	//destruir semaforos y liberación de memoria ocupada
	sem_destroy(&s_estanquero);
	sem_destroy(&sem_fumador_1);
	sem_destroy(&sem_fumador_2);
	sem_destroy(&sem_fumador_3);

  	return 0 ;
}
