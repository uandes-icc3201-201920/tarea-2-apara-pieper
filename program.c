/*
Do not modify this file.
Make all of your changes to main.c instead.
*/

#include "program.h"

#include <stdio.h>
#include <stdlib.h>

static int compare_bytes( const void *pa, const void *pb )
{
	int a = *(char*)pa;
	int b = *(char*)pb;

	if(a<b) {
		return -1;
	} else if(a==b) {
		return 0;
	} else {
		return 1;
	}

}

void access_pattern1( char *data, int length )
{
	// TODO: Implementar
	for (int i = 0; i < length; i++)
	{
		data[i] = 0;
	}
}

void access_pattern2( char *data, int length )
{
	// TODO: Implementar
	//pattern2 sera al azar con la parte que dice en el consejo con lrand48()
	long int pos;
	for( int i = 0; i < length ; i++)
	{
		pos = lrand48()%length;
		data[pos] = 0;
	}
}

void access_pattern3( char *cdata, int length )
{
	// TODO: Implementar
}
