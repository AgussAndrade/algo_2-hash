#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "heap.h"
#define CAP_I 37
#define PORCENTAJE_REDIMENSION 4

struct heap{
	cmp_func_t cmp;
	void** arr;
	size_t cantidad;
	size_t capacidad;
};
int ob_padre(size_t pos){
	return ((int)pos - 1)/2;
}
size_t ob_hijo_i (size_t pos){
	return 2 * pos + 1;
}
size_t ob_hijo_d (size_t pos){
	return 2*pos+2;
}
void swap (void** arr,size_t i,size_t j){
	void* aux = arr[i];
	arr[i] = arr[j];
	arr[j] = aux;
}
void upheap (void** arr,cmp_func_t cmp,size_t inicio,size_t act,size_t final){
	if (act <= inicio || act >= final) return;
	int i = (int)act;
	while (i > (int)inicio){
		int padre = ob_padre(i);
		if (padre <inicio) return;
		if (cmp(arr[i],arr[padre])>0){
			swap(arr,i,padre);
			i = padre;
		}
		else return;
	}
	return;
}
void downheap(void** arr,cmp_func_t cmp,size_t inicio,size_t act,size_t final){
	if (act < inicio || act >= final || inicio == final) return;
	size_t i = act;

	while (i < final){
		size_t hijo_d = ob_hijo_d(i);
		size_t hijo_i = ob_hijo_i(i);
		if(hijo_d < final && hijo_i < final){
			size_t aux = 0;
			if (cmp(arr[hijo_d],arr[hijo_i]) > 0){
				aux = hijo_d;
			}
			else aux = hijo_i;
			if (cmp(arr[aux],arr[i]) > 0 ){
				swap(arr,aux,i);
				i = aux;
				continue;
			}
			else return;
		}
		else if (hijo_d < final){		
			if (cmp(arr[hijo_d],arr[i])>0){
				swap(arr,i,hijo_d);
				i = hijo_d;
				continue;
			}
		}
		else if (hijo_i < final){
			if (cmp(arr[hijo_i],arr[i])>0){
				swap(arr,i,hijo_i);
				i = hijo_i;
				continue;
			}
			return;
		}
		else return;

	}
	return;	
}

void heapify (void** arr,size_t cant,cmp_func_t cmp){
	for (int i = (int)cant - 1 ; i >= 0 ; i--){
		downheap(arr,cmp,0,i,cant);
	}

}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));

	if(!heap) return NULL;
	void** aux = calloc(n*2,sizeof(void*)*2*n);

	if (!aux){
		free(heap);
		return NULL;
	}
	for(int i =0; i<n;i++) aux[i] = arreglo[i];

	heapify(aux,n,cmp);

	heap->cantidad = n;
	heap->capacidad = n*2;
	heap->arr = aux;
	heap->cmp = cmp;
	return heap;

}

heap_t *heap_crear(cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));

	if(!heap) return NULL;
	void** arreglo = calloc(CAP_I,sizeof(void*)* CAP_I);

	if (!arreglo){
		free(heap);
		return NULL;
	}

	heap->cantidad = 0;
	heap->capacidad = CAP_I;
	heap->arr = arreglo;
	heap->cmp = cmp;
	return heap;
}

bool redimensionar (heap_t* heap,size_t num){
	void** arreglo = realloc(heap->arr,sizeof(void*)*num);
	if (!arreglo) return false;

	heap->arr = arreglo;
	heap->capacidad = num;
	return true;
}

size_t heap_cantidad(const heap_t* heap){
	return heap->cantidad;
}

bool heap_encolar(heap_t *heap, void *elem){

	if (heap->cantidad == heap->capacidad){
		bool rta = redimensionar(heap,heap->capacidad*2);

		if (!rta) return false;
	}

	heap->arr[heap->cantidad] = elem;
	upheap(heap->arr,heap->cmp,0,heap->cantidad,heap->cantidad+1);
	heap->cantidad++;
	return true;
}

void *heap_ver_max(const heap_t *heap){
	return heap->arr[0];
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	heapify(elementos,cant,cmp);

	for (int i = (int)cant-1; i>0; i--){
		swap(elementos,0,i);
		downheap(elementos,cmp,0,0,i);
	}
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	void** arr = heap->arr;

	if(destruir_elemento){
		size_t i = 0;

		while(i<heap->cantidad){
			destruir_elemento(arr[i]);
			i++;
		}
	}
	free(arr);
	free(heap);
}

bool heap_esta_vacio(const heap_t *heap){

	if(heap->cantidad == 0) return true;
	return false;
}

void *heap_desencolar(heap_t *heap){

	if((heap->capacidad >= CAP_I) && ((heap->capacidad) / PORCENTAJE_REDIMENSION >= heap->cantidad)){
		redimensionar(heap,heap->capacidad / 2);
	}

	if (heap_esta_vacio(heap)) return NULL;

	if (heap->cantidad == 1){
		void* elem = heap->arr[0];
		heap->arr[0] = NULL;
		heap->cantidad = 0;
		return elem;
	}

	void* elem = heap->arr[0];
	swap(heap->arr,0,heap->cantidad -1);
	heap->arr[heap->cantidad -1] = NULL;
	heap->cantidad --;
	downheap(heap->arr,heap->cmp,0,0,heap->cantidad);

	return elem;
}

