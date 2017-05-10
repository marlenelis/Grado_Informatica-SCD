#include <mpi.h>
#include <iostream>
#include <math.h>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"

// ---------------------------------------------------------------------
// 0 y 1  consumidor
// 2 y 3 produc
// 4 intermedio
//tag

#define Productor     0
#define Intermedio    4
#define Consumidor    1

#define ITERS        10
//
using namespace std;

// ---------------------------------------------------------------------

void productor(int rank)
{ 
int valor;
   for ( unsigned int i= 0 ; i < ITERS ; i++ )
   {      
      valor = rand()%10;       
      cout << "Productor("<< rank <<") produce valor["<< i<< "] = "<<valor << endl << flush;
      // espera bloqueado durante un intervalo de tiempo aleatorio 
      // (entre una décima de segundo y un segundo)
      usleep( 1000U * (100U+(rand()%900U)) );
      //                           al proceso     tag 
      MPI_Ssend( &valor, 1, MPI_INT, Intermedio, Productor, MPI_COMM_WORLD );
   }

}
// ---------------------------------------------------------------------

void PCSIntermedio(int rank)
{
	int   valor,
         peticion;
   MPI_Status status ;  

   for( unsigned int i=0 ; i < ITERS*2 ; i++ )
   {  
         // leer 'status' del siguiente mensaje (esperando si no hay)
        // MPI_Probe( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status );  

            MPI_Recv( &valor, 1, MPI_INT, MPI_ANY_SOURCE, Productor, MPI_COMM_WORLD, &status );
            cout  << "Proceso INTERMEDIO recibe " << valor<< " de Prod. " << status.MPI_SOURCE << endl << flush;        
       
            // Recibe de cualquiera de los consumidores 
            //  MPI_Recv( &peticion, 1, MPI_INT, MPI_ANY_SOURCE, Consumidor, MPI_COMM_WORLD, &status );
        
             if(valor %2 == 0 ){ 
                cout << "Intermedio envia " << valor << " par a Cons. " << 0 << endl << flush;
               //                              al proceso   tag 
                MPI_Ssend( &valor, 1, MPI_INT, 0, Consumidor, MPI_COMM_WORLD);
               
            }else{ 

               cout << "Intermedio envia " << valor << " impar a Cons. " << 1 << endl << flush;
               MPI_Ssend( &valor, 1, MPI_INT, 1, Consumidor , MPI_COMM_WORLD);
            }
   }
   //envio fin a los consumidores para que no queden en espera 
   int fin = -1;
   MPI_Ssend( &fin, 1, MPI_INT, 1, Consumidor , MPI_COMM_WORLD);
   MPI_Ssend( &fin, 1, MPI_INT, 0, Consumidor , MPI_COMM_WORLD);
}  
// ---------------------------------------------------------------------  

void consumidor(int rank)
{
   int         value ; 
  
   MPI_Status  status ;
   int iterador = 1;
   
   while(iterador)
   {
      //                           al proceso   tag 
     // MPI_Ssend(&rank, 1, MPI_INT, Intermedio, Consumidor, MPI_COMM_WORLD); 
      //                           del proceso   tag 
      MPI_Recv(&value, 1, MPI_INT, Intermedio, Consumidor, MPI_COMM_WORLD,&status );
      if(value != -1)      
         cout << "\nConsumidor "<<rank<<" recibe valor " << value << " de Intermedio " << endl << flush ;
      else
         iterador = 0;
      
      // espera bloqueado durante un intervalo de tiempo aleatorio 
      // (entre una décima de segundo y un segundo)
      usleep( 1000U * (100U+(rand()%900U)) ); 
   }
}
// ---------------------------------------------------------------------

int main( int argc, char *argv[] ) 
{
   int rank ,  // identificador de proceso (comienza en 0)
       size ;  // numero de procesos
      
   // inicializar MPI y leer identificador de proceso y número de procesos
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
  
   // inicializa la semilla aleatoria:
   srand ( time(NULL) );
  
   // comprobar el número de procesos con el que el programa 
   // ha sido puesto en marcha (debe ser 3)
   if ( size != 5 ) 
   {  
      cout << "El numero de procesos debe ser 5, pero es " << size << "." << endl << flush ;
      return 0;
   } 
   
   // verificar el identificador de proceso (rank), y ejecutar la
   // operación apropiada a dicho identificador
   if ( rank == Intermedio ) 
      PCSIntermedio(rank);
   else if ( rank == 2 || rank == 3) 
      productor(rank);
   else 
      consumidor(rank);
   
   // al terminar el proceso, finalizar MPI
   MPI_Finalize( );

   return 0;
}
// ---------------------------------------------------------------------
