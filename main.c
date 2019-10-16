/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/
#include <signal.h>
#include "page_table.h"
#include "disk.h"
#include "program.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//Contadores para imprimir resultado
int n_faltas_de_pagina = 0, n_lecturas = 0, n_escrituras = 0;
//valor de argv[3]
char *metodo;
//contiene el arreglo de marco
typedef struct frameArray
{
	int *array;
	int length;
}frameArray;

frameArray *createframeArray(int largo);//funcion para crear arreglo simple de marcos

void page_fault_handler( struct page_table *pt, int page )
{
	printf("page fault on page #%d\n",page);
	n_faltas_de_pagina++;
	
	if(!strcmp(metodo,"FIFO"))
	{
		//funcion metodo reemplazo fifo
	}
	else if (!strcmp(metodo,"rand"))
	{
		//funcion metodo reemplazo random
	}
	exit(1);
}

int main( int argc, char *argv[] )
{
	if(argc!=5) {
		printf("use: virtmem <npages> <nframes> <lru|fifo> <access pattern>\n");
		return 1;
	}

	int npages = atoi(argv[1]);
	int nframes = atoi(argv[2]);
	const char *program = argv[4];
	metodo = argv[3];
//printf("%d \n",*metodo);
//printf("%d \n",*program);
	
	frameArray *tabla_marcos = createframeArray(nframes);//guardo la tabla de marco,array simple
	//valoro en 0 la tabla
	for( int i=0; i < tabla_marcos->length ; i++)
	{
		tabla_marcos->array[i] = 0;
		printf("marco %d data %d\n",i,tabla_marcos->array[i]);
	}

	struct disk *disk = disk_open("myvirtualdisk",npages);
	if(!disk) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}


	struct page_table *pt = page_table_create( npages, nframes, page_fault_handler );
	if(!pt) {
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	}

	char *virtmem = page_table_get_virtmem(pt);

	char *physmem = page_table_get_physmem(pt);

	//page_table_set_entry(pt,npages,npages,PROT_READ|PROT_WRITE);//ACA SE CAE! <----------

	if(!strcmp(program,"pattern1")) {
		access_pattern1(virtmem,npages*PAGE_SIZE);
		disk_write( disk, npages, physmem );

	} else if(!strcmp(program,"pattern2")) {
		access_pattern2(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"pattern3")) {
		access_pattern3(virtmem,npages*PAGE_SIZE);

	} else {
		fprintf(stderr,"unknown program: %s\n",argv[3]);

	}

	page_table_delete(pt);
	disk_close(disk);
	
	free(tabla_marcos);
	
	//resultados
	printf("         -----------> Input <-----------\n");
	printf("   Algoritmo de reemplazo de pagina:   %s\n",metodo);
	printf("   Tipo de acceso a memoria utilizado: %s\n",program);
	printf("   Cantidad de paginas:                %d\n",npages);
	printf("   Cantidad de marcos:                 %d\n\n",nframes);
	printf("         -----------> Resumen <-----------\n");
	printf("   Faltas de pagina:                   %d\n   Lecturas de disco:                  %d\n   Escrituras a disco:                 %d\n\n",n_faltas_de_pagina,n_lecturas,n_escrituras);

	return 0;
}

frameArray *createframeArray(int largo)
{
	frameArray *newArray = malloc(sizeof(frameArray));
	newArray->length = largo;
	newArray->array = malloc(sizeof(int)*largo);
	return newArray;
}
