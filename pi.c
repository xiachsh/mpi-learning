#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>


#include <mpi.h>

void usage(int argc,char **argv)
{
	fprintf(stderr,"%s usage:\n",argv[0]);
	fprintf(stderr,"	:%s iterations\n",argv[0]);
	exit(1);
}

int main(int argc,char ** argv)
{
	int size;
	int rank;



	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if (argc != 2 && rank == 0)
		usage(argc,argv);

	unsigned long int * sendBuff = (unsigned long int *)malloc(sizeof(unsigned long int));
	unsigned long int * recvBuff = (unsigned long int *)malloc(sizeof(unsigned long int) * size);
	unsigned long int iteration = atol(argv[1]);
	unsigned long int i = 0;
	unsigned long int count = 0;
	long long int total = 0;
	
	srand(time(NULL));
	
	for (i=0;i<iteration;i++)
	{
		float x = (float) rand() / RAND_MAX;
		float y = (float) rand() / RAND_MAX;
//		fprintf(stdout,"x:%f y:%f x**x + y**y :%f\n", x,y,x*x + y*y );	
		if( x*x + y*y <= 1)
			count++;
	}
	sendBuff[0] = count;
	
	MPI_Gather(sendBuff,1,MPI_LONG,recvBuff,1,MPI_LONG,0,MPI_COMM_WORLD);	

	if (rank == 0) {
		for (i=0;i<size;i++){
			total = total + recvBuff[i];
		}
		float pi = 4 * (float) total / (size * iteration);
		fprintf(stdout,"PI is roughly about %1.20f\n",pi);
	}
	free(sendBuff);
	free(recvBuff);
	MPI_Finalize();
	

	return 0;
}
