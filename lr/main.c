#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>


#include "data.h"
#include <mpi.h>


void usage(int argc,char **argv)
{
    fprintf(stderr,"%s usage:\n",argv[0]);
    fprintf(stderr,"    : %s dataFile\n",argv[0]);
    exit(1);
}

int main(int argc,char **argv)
{

        int size,rank;
        int x = 0;
        Point * pnts = NULL;
    
        Point * recvBuff = NULL;

    
        MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD,&size);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);

        if (rank == 0 && argc != 2) {
            MPI_Finalize();
            usage(argc,argv);
        }

        if (rank == 0)
             pnts = readDataFromFile(argv[1],&x);
        else 
             pnts = malloc(sizeof(Point)  * MAX_RETURN_POINT );
        recvBuff = malloc(sizeof(Point) * MAX_RETURN_POINT * size);
        MPI_Bcast(pnts,sizeof(Point) * MAX_RETURN_POINT,MPI_CHAR,0,MPI_COMM_WORLD);


        if (rank == size-1) {
             
            int i = 0;
            for (i=0;i<MAX_RETURN_POINT;i++) {
                fprintf(stdout,"%f\t%f\n",pnts[i].x,pnts[i].y);
            }
        }
    
         

        free(pnts);
        free(recvBuff);
        MPI_Finalize();
        return 0 ;
}
