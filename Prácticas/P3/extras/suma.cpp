/*

suma de un vector con paso de mensajes

P0----------P1----------P3
|			|			|
|			|			|
P2----------P4----------P5
|			|			|
|			|			|
P6----------P7----------P8

*/

#include <mpi.h>
#include <iostream>
#include <math.h>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"

using namespace std;

sumar(int rank , int valor){
	
}

int main (int argc, char *argv[]){ 

	int rank,
		size;
	int v[]= {1,2,3,4,5,6,7,8,9}
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	for (int i = 0; i < 9; ++i)
	{
		valor = v[i];
		sumar(rank, valor);
	}

}