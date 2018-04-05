#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>


int main()
{

	int size,rank;
	
		
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);


	char file[256];
	char buf[256];
	sprintf(file,"file%d",rank);

	int i = 0;
	for (i = 0 ; i<256;i++ ) {
		buf[i] = 'A' + rank;
	}

	MPI_File fh;
	MPI_Status status;
	MPI_File_open(MPI_COMM_SELF,file,MPI_MODE_CREATE|MPI_MODE_WRONLY,MPI_INFO_NULL,&fh);
	MPI_File_write(fh,buf,256,MPI_CHAR,&status);
	MPI_File_close(&fh);
	MPI_Finalize();
	return 0;
}
