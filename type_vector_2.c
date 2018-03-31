#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <mpi.h>


#define N 16


void print_matrix(int * arr,int row,int col)
{
	int i,j;

	for (i=0;i<row;i++)  {
		for (j=0;j<col;j++) {
			printf("%3d\t",arr[i*col + j]);
		}
		printf("\n");
	}
}


void usage(int argc,char **argv)
{
	fprintf(stderr,"%s usage:\n",argv[0]);
	fprintf(stderr,"	%s index\n",argv[0]);
	MPI_Abort(MPI_COMM_WORLD,1);
	exit(1);
}

int main(int argc,char **argv)
{
	int size,rank;
	
	int offset = 0;
	MPI_Init(NULL,NULL);

	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	

	MPI_Datatype newtype;

	int A[N][N];
	if (rank == 0 ) {
		if (argc == 1) 
			usage(argc,argv);
		else 
			offset = atoi(argv[1]);
			
		int i,j;
		for (i = 0;i < N;i++) 
			for (j = 0;j < N;j++) {
			A[i][j] = i * N + j;
		}
		
	}
	else 
		bzero(A,sizeof(int) * N * N);
	MPI_Type_vector(N,1,N,MPI_INT,&newtype);
	MPI_Type_commit(&newtype);
	

	//some problem to fix
	MPI_Bcast(A+offset,1,newtype,0,MPI_COMM_WORLD);
	if (rank == 1) 
		print_matrix(A,N,N);

	MPI_Type_free(&newtype);
	MPI_Finalize();

	return 0;
}
