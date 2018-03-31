#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#include <mpi.h>

#define CHECK_MPI_ERR(x)  					\
	if (  (rc = x) != MPI_SUCCESS) { 			\
		char buf[256];					\
		int len = 0 ;					\
		MPI_Error_string(rc,buf,&len);			\
		printf("MPI_CALL line:%d failed with error message %s\n", __LINE__,buf);\
		MPI_Abort(MPI_COMM_WORLD,1);			\
		exit(1);					\
	}

typedef struct {
	char c;
	int i;
	float f;
	double d;
} myStruct;

int main()
{
	int size,rank;
	int rc;

	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	MPI_Datatype newtype;
	int blocks[4] = {1,1,1,1};
	MPI_Datatype types[4] = {MPI_CHAR,MPI_INT,MPI_FLOAT,MPI_DOUBLE};
	myStruct s;
	MPI_Aint disps[4] = {0,0,0,0};
	MPI_Get_address(&(s.c),&(disps[0]));
	MPI_Get_address(&(s.i),&(disps[1]));
	MPI_Get_address(&(s.f),&(disps[2]));
	MPI_Get_address(&(s.d),&(disps[3]));

	disps[3] -= disps[0]; 
	disps[2] -= disps[0]; 
	disps[1] -= disps[0]; 
	disps[0] = 0;
	
	
	MPI_Type_create_struct(4,blocks,disps,types,&newtype);
	MPI_Type_commit(&newtype);

	if (rank == 0)  
	{
		s.c = 'A';
		s.i = 232;
		s.f = 2.2f;
		s.d = 1.1000f;
	}
	MPI_Bcast(&s,1,newtype,0,MPI_COMM_WORLD);	
	
	if (rank == 3)
	printf("rank:%d\ts.c:%c\ts.i:%d\ts.f:%f\ts.d:%lf\n",rank,s.c,s.i,s.f,s.d);

	MPI_Type_free(&newtype);
	MPI_Finalize();
	return 0;
}
