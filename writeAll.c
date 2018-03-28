#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include <mpi.h>
 

#define N 1024

int main(int argc,char **argv)
{
	int size;
	int rank;


	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	if (rank == 0) {
		printf("argc:%d\n",argc);

		if (argc < 2) {

			printf("%s usage\n",argv[0]);
			printf("	%s fileName\n",argv[0]);
		
			MPI_Abort(MPI_COMM_WORLD,1);
			exit(1);
		}
	} 
	char *buf = malloc(sizeof(char) * N);

	int i = 0;
	for (i = 0; i < N;i++) {
		buf[i] = 'A';
	}
	
	MPI_File fh;
	MPI_File_open(MPI_COMM_WORLD,argv[1],MPI_MODE_CREATE|MPI_MODE_WRONLY,MPI_INFO_NULL,&fh);
	
	MPI_File_write_all(fh,buf,N,MPI_CHAR,NULL);

	MPI_File_close(&fh);


	MPI_Finalize();

	return 0;
}
