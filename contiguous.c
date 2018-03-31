#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>


int main()
{
	int size,rank;
	
	MPI_Datatype newtype;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	
	
	//MPI_Type_contiguous(16,MPI_DOUBLE,&newtype);
	MPI_Type_contiguous(64,MPI_DOUBLE,&newtype);
	MPI_Type_commit(&newtype);

	double A[128];

	if (rank == 0)
		A[32] = 1.2323f;


	int rc = MPI_Bcast(A,1,newtype,0,MPI_COMM_WORLD);
	if (rc != MPI_SUCCESS) {
		char buf[256];
		int len = 0;
		MPI_Error_string(rc,buf,&len);	
		MPI_Abort(MPI_COMM_WORLD,1);
		exit(1);
	}


	MPI_Barrier(MPI_COMM_WORLD);
	printf("rank :%d value :%f\n",rank,A[32]);

		
	MPI_Type_free(&newtype);
	MPI_Finalize();
	return 0;
}
