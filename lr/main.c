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
        int pointCnt = 0;
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
             pnts = readDataFromFile(argv[1],&pointCnt);
        else 
             pnts = malloc(sizeof(Point)  * MAX_RETURN_POINT );
        recvBuff = malloc(sizeof(Point) * MAX_RETURN_POINT);
        bzero(recvBuff,sizeof(Point) * MAX_RETURN_POINT);
        int sendSize = sizeof(Point) * MAX_RETURN_POINT / size;
        MPI_Scatter(pnts,sendSize,MPI_CHAR,recvBuff,sendSize,MPI_CHAR,0,MPI_COMM_WORLD);
        
        long double sumX = 0.0f, sumY = 0.0f, sumXMultiX = 0.0f,sumXMultiY = 0.0f;
        
        long double * resultBuff = malloc(sizeof(long double) * 4);
        long double * recvRstBuff = malloc(sizeof(long double) * 4 * size);

             
            int i = 0;
            for (i=0;i<MAX_RETURN_POINT;i++) {
                sumX += recvBuff[i].x;
                sumY += recvBuff[i].y;
                sumXMultiX += ((recvBuff[i].x) * (recvBuff[i].x));
                sumXMultiY += ((recvBuff[i].x) * (recvBuff[i].y));
            }
    
        resultBuff[0] = sumX;
        resultBuff[1] = sumY;
        resultBuff[2] = sumXMultiX;
        resultBuff[3] = sumXMultiY;
         

        MPI_Gather(resultBuff,4,MPI_LONG_DOUBLE,recvRstBuff,4,MPI_LONG_DOUBLE,0,MPI_COMM_WORLD);
        long double slop = 0.0f;
        long double aix = 0.0f;
        sumX = 0.0f; sumY = 0.0f; sumXMultiX = 0.0f;sumXMultiY = 0.0f;
        if (rank == 0) {
            int y = 0;
            for (;y<4;y++) {
                sumX += recvRstBuff[4*y+0];
                sumY += recvRstBuff[4*y+1];
                sumXMultiX += recvRstBuff[4*y+2];
                sumXMultiY += recvRstBuff[4*y+3];
            }

            slop = (sumXMultiY - sumY*sumX)  / (sumXMultiX - (sumX*sumX));
            aix = ( sumY - (slop * sumX) ) / pointCnt;

            printf("y = %4.10Le + %4.10Le * x\n",aix,slop);
        }

        free(resultBuff);
        free(recvRstBuff);
        free(pnts);
        free(recvBuff);
        MPI_Finalize();
        return 0 ;
}
