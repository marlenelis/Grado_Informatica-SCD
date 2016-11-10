// *****************************************************************************
//
// Ejercicio extra.
// Suma de los elementos pares e impares de un vector, cada suma la realizan hebras distintas.
//
// Autor:			Marlene E. Vásquez
// Compilar: 		g++ -o suma suma_vector.cpp -lpthread
// Uso: 			./suma
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

//declaracion de semaforos
sem_t 	escribir_P1,
		escribir_P2,
		sem_P1,
		sem_P2,
		mutex;
//declaracion de hebras
pthread_t 	h_generadora,
			h_pares,
			h_impares;

//declaracion de variables 
const int N=300; // tamaño del buffer
int buffer[N];// vector

int	resul_pares, // variable para almacenar el resultado de sumar los numeros pares
	resul_impares;// variable para almacenar el resultado de sumar los numeros impares

// ---------------------------------------------------------------------
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

//===================================================================
//funcion que genera valores de 0 a 4 insertando esos valores en el buffer de tamaño N 

void * funcion_generador(void *){
	 int sumatotal;
	while(true){
		
			for (int i = 0; i < N; ++i)		
				buffer[i] = rand()%11+12; //genera valores y los almacena en el buffer

		sem_post(&sem_P1);// libera P1
		sem_post(&sem_P2);// libera P2

		sem_wait(&escribir_P1); // espera a que se pueda escribir ( signal desde P1)
		sem_wait(&escribir_P2); // espera a que se pueda escribir ( signal desde P2)	
			sem_wait(&mutex);	
			retraso_aleatorio( 0.1, 0.5 );		    	   
			sumatotal = resul_pares + resul_impares;			
			cout<<"Suma total = " << sumatotal <<"\n"<<endl<<flush;
			sem_post(&mutex);
		
			
		
	}
}
//===================================================================
// funcion que  suma los numeros pares del vector de tamaño N

void * funcion_sumador_pares(void *){
int aux;
	while(true){
		
		aux=0;		
		
		sem_wait(&sem_P1); // espera  a que se pueda leer (signal desde P0)

			for (int i = 0; i < N; ++i)
				if(buffer[i]%2==0){// se  comprueba si el numero es par
					
				sem_wait(&mutex);
					aux+= buffer[i];// acumulador de la suma par
					resul_pares =aux;
				sem_post(&mutex);
}	
			cout << "Par: resultado = " << resul_pares << endl <<flush; // imprime el resultado
		
		sem_post(&escribir_P1);		//libera al generador (P0), puede escribir
	}
}
//===================================================================
// funcion que  suma los numeros impares del vector de tamaño N

void * funcion_sumador_impares(void *){
	int aux;
	while(true){

		aux=0;
		
		sem_wait(&sem_P2);// espera  a que se pueda leer (signal desde P0)
		
			for (int i = 0; i < N; ++i)
				if(buffer[i]%2!=0){// se  comprueba si el numero es impar
					
					sem_wait(&mutex);
					aux+= buffer[i]; // acumulador de la suma impar
					resul_impares =aux;
				sem_post(&mutex);
				}
			cout << "Impar: resultado = " << resul_impares << endl <<flush;		// imprime el resultado	
		sem_post(&escribir_P2);	//libera al generador (P0), puede escribir
	}
}
//===================================================================
int main(int argc, char const *argv[])
{
	//inicializacion de los semaforos
	sem_init(&escribir_P1,0,0);//
	sem_init(&escribir_P2,0,0);//
	sem_init(&sem_P2,0,0);// inicia a 0, espera a que se genere los datos para realizar la lectura
	sem_init(&sem_P1,0,0);// inicia a 0, espera a que se genere los datos para realizar la lectura
	sem_init(&mutex,0,1);
	//creacion de las hebras
	pthread_create(&h_generadora, 	NULL, funcion_generador,		NULL);// P0
	pthread_create(&h_pares, 		NULL, funcion_sumador_pares,	NULL);// P1
	pthread_create(&h_impares, 		NULL, funcion_sumador_impares, 	NULL);// P2

	// espera de las hebras
	pthread_join(h_generadora, 	NULL);
	pthread_join(h_impares,		NULL);
	pthread_join(h_pares, 		NULL);	
	
	// destuye los semaforos y liberacion de memoria ocupada
	sem_destroy(&escribir_P1);
	sem_destroy(&escribir_P2);
	sem_destroy(&sem_P1);
	sem_destroy(&sem_P2);

	return 0;
}
