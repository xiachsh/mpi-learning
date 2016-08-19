#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>


#include "data.h"

Point * readDataFromFile(const char * path,int * ptCnt)
{
	int fd = open(path,O_RDONLY);
	char * buff = malloc(sizeof(char) * MAX_DATA_BUF);
	Point * retPoint = (Point *) malloc(sizeof(Point) * MAX_RETURN_POINT);
	int pointCnt = 0 ;

	if (fd == -1) {
		fprintf(stderr,"open file %s failed,please check\n",path);
		return NULL;
	}

	

	struct stat statbuf;

	stat(path,&statbuf);
	int fileSize = statbuf.st_size;	

	bzero(buff,sizeof(char) * MAX_DATA_BUF);
	bzero(retPoint,sizeof(Point) * MAX_RETURN_POINT);
	if (fileSize > MAX_DATA_BUF) {

	}
	else {
		read(fd,buff,MAX_DATA_BUF);
		char * tmpBuff = buff;
		for (;;tmpBuff=NULL) {
                        float x,y;
			char * token;
			char * savePtr1;
			token = strtok_r(tmpBuff," \t\n",&savePtr1);
			char * subtoken, *savePtr2;
			if (token == NULL)	
				break;

                        char * _str =  token;
                        subtoken = strtok_r(_str,",",&savePtr2);
                        x = atof(subtoken+1);
                        
                        _str = NULL;
                        subtoken = strtok_r(_str,",",&savePtr2);
                        y = atof(subtoken);
    
                        Point pnt = {x,y};
                        retPoint[pointCnt++] = pnt;
		}
	}
        *ptCnt = pointCnt;

	free(buff);
	
	return retPoint;
}

