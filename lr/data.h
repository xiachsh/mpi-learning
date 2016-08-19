#ifndef __DATA_H
#define __DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>


#define MAX_DATA_BUF 16 * 1024 * 1024
//#define MAX_RETURN_POINT 1024 * 1024
#define MAX_RETURN_POINT  5120

typedef struct _point {
	float x;
	float y;
} Point;


Point * readDataFromFile(const char * path,int *ptCnt);

#endif 
