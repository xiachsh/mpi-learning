#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>


typedef struct {
	double d;
	char c;
} myStruct ;


#define N 32

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
	disps[0] = 0;
	MPI_Type_create_struct(2,blocks,disps,types,&newtype); 
	MPI_Type_commit(&newtype);
	myStruct structArr[N];

	

	MPI_Datatype newtype_1 ;
	MPI_Type_vector(2,3,10,newtype,&newtype_1);
	MPI_Type_commit(&newtype_1);
	int _size ;
	int _size_1;
	MPI_Type_size(newtype,&_size);
	MPI_Type_size(newtype_1,&_size_1);
	
	 

	
	if (rank == 0)
	printf("sizeof origin struct is %d sizeof newtype is %d sizeof newtype_1 is %d\n",sizeof(myStruct),_size,_size_1);


	if (rank == 0) {
		int i = 0;
		for (;i<N;i++) {
			structArr[i].d = 1.0f * i ;
			structArr[i].c = 'A' + i;
		}
		
	}

	MPI_Bcast(structArr,1,newtype_1,0,MPI_COMM_WORLD);

	if (rank == 1) {
                int i = 0;
                for (;i<N;i++) {
			char c = structArr[i].c < 'A' + 32 && structArr[i].c > 'A' -1 ? structArr[i].c : ' ';
			printf("s.c:%f s.d:%c\n",structArr[i].d,c);
                }

        }

	MPI_Type_free(&newtype);
	MPI_Finalize();
	return 0 ;
}
