#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>


typedef struct {
	double d;
	char c;
} myStruct ;

int main()
{
	int size,rank;
	
	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	
	myStruct s;
	MPI_Datatype newtype;
	MPI_Datatype types[2] = {MPI_DOUBLE,MPI_CHAR};
	int blocks[2] = {1,1};
	MPI_Aint disps[2] = {0,0};
	MPI_Get_address(&s.d,&disps[0]);
	MPI_Get_address(&s.c,&disps[1]);

	disps[1] -= disps[0];
	MPI_Type_create_struct(2,blocks,disps,types,&newtype); 
	MPI_Type_commit(&newtype);


	MPI_Datatype newtype_1 ;
	MPI_Type_vector(2,3,10,newtype,&newtype_1);
	int _size ;
	int _size_1;
	MPI_Type_size(newtype,&_size);
	MPI_Type_size(newtype_1,&_size_1);

	if (rank == 0)
	printf("sizeof origin struct is %d sizeof newtype is %d sizeof newtype_1 is %d\n",sizeof(myStruct),_size,_size_1);

	MPI_Type_free(&newtype);
	MPI_Finalize();
	return 0 ;
}
