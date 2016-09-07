#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>


#include <mpi.h>
#define LEN 128



/* the following is the simplest one-sided communication call MPI_Put */

int main(int argc,char ** argv)
{
	int size;
	int rank;

	char buf[LEN];
	MPI_Group group;
	MPI_Win win;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	memset(buf,0,LEN);


	MPI_Comm_group(MPI_COMM_WORLD,&group);
	MPI_Win_create(buf,LEN,sizeof(char),MPI_INFO_NULL,MPI_COMM_WORLD,&win);
	
	/* must put a MPI_Win_fence here otherwise MPI_Put will raise an error */	
	MPI_Win_fence(0,win);
	if (rank == 0) {
		sprintf(buf,"hello world from rank %d\n",rank);
		MPI_Put(buf,LEN,MPI_CHAR,1,0,LEN,MPI_CHAR,win);
	}	
	MPI_Win_fence(0,win);
	printf("rank :%d buf: %s",rank,buf);
	
	MPI_Win_free(&win);	
	MPI_Finalize();
	return 0;
}
