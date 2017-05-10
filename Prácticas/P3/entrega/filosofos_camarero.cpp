#include <iostream>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"
#include <mpi.h>

using namespace std;

//definir tags
#define sentarse 0
#define levantarse 1

#define coger 2
#define soltar 3

#define camarero 10

void Filosofo( int id, int nprocesos);
void Tenedor ( int id, int nprocesos);
void Camarero ( int id);

// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int rank, size;
   
   srand(time(0));
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
   
   if( size!=11)
   {
      if( rank == 0) 
         cout<<"El numero de procesos debe ser 10" << endl << flush ;
      MPI_Finalize( ); 
      return 0; 
   }
   
   
   if ((rank%2) == 0)  
	if(rank ==10){//proceso 10
		Camarero(rank);
   	}else
      		Filosofo(rank,size); // Los pares son Filosofos 
   else 
      Tenedor(rank,size);  // Los impares son Tenedores 
   
   MPI_Finalize( );
   return 0;
}  
// ---------------------------------------------------------------------


void Camarero( int id )
{
  MPI_Status status;
  int max = 4, sentados = 0,buf, Filo, tag;

  while(true){
    // solo pueden sentarse como max 4 filosofos
    if(sentados < max )
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status); //Espera cualquier señal tag levantarse o sentarse
    else
      MPI_Probe(MPI_ANY_SOURCE, levantarse, MPI_COMM_WORLD, &status); //Espera solo mensaje con tag levantarse    
    
    tag = status.MPI_TAG ; 
    buf = status.MPI_SOURCE;

    if (tag == sentarse){      
      MPI_Recv(NULL,0, MPI_INT, buf, sentarse, MPI_COMM_WORLD,&status);
      sentados++; 

      MPI_Send(NULL,0, MPI_INT,buf, sentarse, MPI_COMM_WORLD);
      cout << "Filosofo " << buf << " se sienta. Hay " << sentados << " filosofos sentados. " << endl;
    }

    if(tag == levantarse){
      MPI_Recv(NULL,0, MPI_INT, buf, levantarse, MPI_COMM_WORLD,&status);
      sentados--;
      cout <<"El filosofo "<<buf<<" se levanta de la mesa. Hay "<< sentados<<" filosofos sentados. " << endl;
    }    
  }
}
// ---------------------------------------------------------------------


void Filosofo( int id, int nprocesos )
{
   int izq = (id+1) % (nprocesos-1);
   int der = (id+nprocesos-2) % (nprocesos-1);   
   
   while(true)
   {

     //Solicita sitio en la mesa.
      cout << "Filosofo "<<id<<" solicita sentarse en la mesa"<<endl<<flush;
      MPI_Send(NULL, 0,MPI_INT,camarero,sentarse,MPI_COMM_WORLD); //Solicita al camarero (proceso 10) sentarse en la mesa (flag sentarse)


      // Solicita tenedor izquierdo
      cout << "Filosofo "<<id<< " solicita tenedor izq ..." << izq << endl << flush;
      // ...
       MPI_Ssend(NULL, 0, MPI_INT, izq, coger, MPI_COMM_WORLD);
      // Solicita tenedor derecho
      cout <<"Filosofo "<<id<< " coge tenedor der ..." << der << endl << flush;
      // ...
       MPI_Ssend(NULL, 0, MPI_INT, der, coger, MPI_COMM_WORLD);       
    
      cout<<"Filosofo "<<id<< " COMIENDO"<<endl<<flush;
      sleep((rand() % 3)+1);  //comiendo
      
      // Suelta el tenedor izquierdo
      cout <<"Filosofo "<<id<< " suelta tenedor izq ..." << izq << endl << flush;
      // ...
       MPI_Ssend(NULL, 0, MPI_INT, izq, soltar, MPI_COMM_WORLD);
      // Suelta el tenedor derecho
      cout <<"Filosofo "<<id<< " suelta tenedor der ..." << der << endl << flush;
      // ...
      MPI_Ssend(NULL, 0, MPI_INT, der, soltar, MPI_COMM_WORLD);

     //solicita levantarse
      MPI_Ssend(NULL, 0,MPI_INT,camarero,levantarse,MPI_COMM_WORLD);
      cout << "Filosofo "<<id<<" se levanta de la mesa"<<endl<<flush;

      // Piensa (espera bloqueada aleatorio del proceso)
      cout << "Filosofo " << id << " PENSANDO" << endl << flush;
      
      // espera bloqueado durante un intervalo de tiempo aleatorio 
      // (entre una décima de segundo y un segundo)
      usleep( 1000U * (100U+(rand()%900U)) );
 }
}
// ---------------------------------------------------------------------

void Tenedor(int id, int nprocesos)
{
  int buf; 
  MPI_Status status; 
  int Filo;
  int flag;
  
  while( true )
  { 
    // Recibe la peticion del filosofo ...
      MPI_Recv(&buf,1,MPI_INT,MPI_ANY_SOURCE,coger,MPI_COMM_WORLD, &status);

      Filo = status.MPI_SOURCE;

      cout<<"Tenedor "<<id<<" recibe peticion de "<< Filo << endl << flush;
      // Espera a que el filosofo suelte el tenedor...

      MPI_Recv(&Filo, 1,MPI_INT,Filo,soltar,MPI_COMM_WORLD,&status);
      
      cout<<"Tenedor "<<id<<" recibe liberacion de "<<Filo<<endl<<flush;
    }
  //}
}
// ---------------------------------------------------------------------
