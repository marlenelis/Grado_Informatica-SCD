// *****************************************************************************
//
// Prácticas de SCD. Práctica 1.
// Ejercicio del productor-consumidor con buffer intermedio gestionado con la alternativa FIFO(cola circular)
//
// Autor:			Marlene E. Vásquez
// Compilar: 		g++ -o prod-cons prod-cons_FIFO.cpp -lpthread
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

//declaración de los semáforos
sem_t 	puede_escribir,
		puede_leer_par, puede_leer_impar,
		mutex;

int buffer[tam_vector];//buffer- vector intermedio
int buffer_par[num_items];//buffer para elementos pares
int buffer_impar[num_items];//buffer para elementos impares
int contador_pares=0;
int contador_impares=0;
int aux;
int caux;
//variables para la gestión de la ocupación.
int 	primera_libre   = 0, // esta variable se incrementa al escribir(módulo tam_vector)

	primera_ocupada = 0,// esta variable se incrementa al leer (módulo tam_vector).
	primera_libre_II  = 0, // esta variable se incrementa al escribir(módulo tam_vector)

	primera_ocupada_II = 0; // esta variable se incrementa al leer (módulo tam_vector).

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
  contador = rand()%21+10 ;//producir dato entre 10 y 30
  retraso_aleatorio( 0.1, 0.5 );
  cout << "Productor : dato producido: " << contador << endl << flush ;
  return contador ;
}
// ---------------------------------------------------------------------
// función que simula la consumición de un dato

void consumir_dato( int dato )
{
   retraso_aleatorio( 0.1, 1.5 );
   cout << "Consumidor:\t\t\tdato consumido: " << dato << endl << flush ;
}
// ---------------------------------------------------------------------
// función que ejecuta la hebra del productor

void * funcion_productor( void * )
{
	contador_impares=0;
	contador_pares = 0;

  for( unsigned i = 0 ; i < num_items ; i++ )
  {
    	int dato = producir_dato() ;
		sem_wait(&puede_escribir);
			sem_wait(&mutex);
			    //insertar "dato" en el vector o buffer
				if(dato%2==0){
					buffer_par[primera_libre] = dato;
					primera_libre = (primera_libre+1)%tam_vector; 
					
					contador_pares++;

					cout << "Productor : \033[1;31m dato par insertado: \033[0m" << dato<<"\tc_p: "<<contador_pares  << endl << flush ;
					sem_post(&puede_leer_par);
				}
				else{
				 buffer_impar[primera_libre_II] = dato;
				 primera_libre_II = (primera_libre_II+1)%tam_vector; 
					
					contador_impares++;
					cout << "Productor : \033[1;36m dato impar insertado: \033[0m" << dato<<"\tc_i: "<<contador_impares << endl << flush ;
					sem_post(&puede_leer_impar);
				}
			    
			sem_post(&mutex);		
  }
  cout << "Productor : \033[1;36m Nº de datos impares insertados: \033[0m" << contador_impares << endl << flush ;
  cout << "Productor : \033[1;36m Nº de datos pares insertados: \033[0m" << contador_pares << endl << flush ;
  return NULL ;
}
// ---------------------------------------------------------------------
// función que ejecuta la hebra del consumidor

void * funcion_consumidor( void * )
{	
	aux = 0;

  
  do
  {

    int dato ;
	sem_wait(&puede_leer_par);	
		sem_wait(&mutex); 
		    //leer "dato" desde el vector intermedio
			dato = buffer_par[primera_ocupada];
		   	primera_ocupada = (primera_ocupada+1)%tam_vector;
			aux++;
	    	cout << "Consumidor 2:\t\t\t\033[1;31m dato extraído : \033[0m" << dato << endl << flush ;
	    	cout << "Consumidor 1:\t\t\t\033[1;33m aux : \033[0m" << aux << endl << flush ;	
	   	sem_post(&mutex);	
	sem_post(&puede_escribir);
    consumir_dato( dato ) ;
  }while(contador_pares>aux);

  return NULL ;
}

void * funcion_consumidor_IMPAR( void * )
{
	caux = 0;
  
  
 do {
    int dato ;
	sem_wait(&puede_leer_impar);

		sem_wait(&mutex); 
		    //leer "dato" desde el vector intermedio
			dato = buffer_impar[primera_ocupada_II];
		   	primera_ocupada_II = (primera_ocupada_II+1)%tam_vector;
			caux++;
	    	cout << "Consumidor 1:\t\t\t\033[1;36m dato extraído : \033[0m" << dato << endl << flush ;
	    	cout << "Consumidor 1:\t\t\t\033[1;36m caux : \033[0m" << caux << endl << flush ;				
	   	sem_post(&mutex);

	sem_post(&puede_escribir);
    consumir_dato( dato ) ;
  }while(contador_impares>caux );


  return NULL ;
}
//----------------------------------------------------------------------

int main()
{
	//declaracion de las hebras.	
	pthread_t 	hebra_escritora,
			hebra_lectora,
			hebra_lectora_impar;
	//inicio de los semaforos
	sem_init(&puede_escribir,0,tam_vector);
	sem_init(&puede_leer_par,	 0,0);
	sem_init(&puede_leer_impar,	 0,0);
	sem_init(&mutex,0,1);
	//creación de las hebras
	pthread_create(&hebra_escritora,NULL, funcion_productor, NULL);
	pthread_create(&hebra_lectora, 	NULL, funcion_consumidor,NULL);
	pthread_create(&hebra_lectora, 	NULL, funcion_consumidor_IMPAR,NULL);
	//espera de las hebras
	pthread_join(hebra_escritora,  NULL);

pthread_join(hebra_lectora,    NULL);
pthread_join(hebra_lectora_impar,    NULL);	
	
	
	//pthread_exit(NULL);
	cout<< "FIN"<<endl;

	//destruye los semaforos y libera la memoria ocupada
	sem_destroy(&puede_escribir);
	sem_destroy(&puede_leer_par);
	sem_destroy(&puede_leer_impar);
	sem_destroy(&mutex);
 
   return 0 ;
}
