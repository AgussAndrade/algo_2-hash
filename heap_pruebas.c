#include "heap.h"
#include "testing.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Prototipo de función de comparación que se le pasa como parámetro a las
 * diversas funciones del heap.
 * Debe recibir dos punteros del tipo de dato utilizado en el heap, y
 * debe devolver:
 *   menor a 0  si  a < b
 *       0      si  a == b
 *   mayor a 0  si  a > b
 */
int cmp_func(const void *a, const void *b){
	if (*(int*)a < *(int*)b ) return -1;
	if (*(int*)a == *(int*)b ) return 0;
	return 1;
}

void** arreglo(){
	void** arr = malloc(sizeof(void*) * 5);
	int* n4 = malloc(sizeof(int));
	int* n2 = malloc(sizeof(int));
	int* n9 = malloc(sizeof(int));
	int* n1 = malloc(sizeof(int));
	int* n20 = malloc(sizeof(int));
	*n4 = 4, *n2 = 2, *n9 = 9, *n1 = 1, *n20 = 20;
	arr[0] = n4;
	arr[1] = n2;
	arr[2] = n9;
	arr[3] = n1;
	arr[4] = n20;
	return arr;
}

void pruebas_heap_vacio(){
	heap_t* heap = heap_crear(cmp_func);
	print_test("El heap fue creado correctamente",heap != NULL);
	print_test("No puedo desencolar en una heap vacío",heap_desencolar(heap) == NULL);
	print_test("No puedo ver el máximo de un heap vacío",heap_ver_max(heap) == NULL);
	print_test("La cantidad de un heap vacío es 0",heap_cantidad(heap) == 0);
	print_test("Compruebo si el heap esta vacío",heap_esta_vacio(heap) == true);
	heap_destruir(heap,NULL);
	print_test("El heap fue destruido correctamente",true);
}

void pruebas_comportamiento_heap() {
	heap_t* heap = heap_crear(cmp_func);
	int valor1 = 4, valor2 = 3, valor3 = 2, valor4 = 1;
	int* elemento1 = &valor1;
	int* elemento2 = &valor2; 
	int* elemento3 = &valor3; 
	int* elemento4 = &valor4;
	print_test("Se encolo el elemento1",heap_encolar(heap,elemento1)== true);
	print_test("Se encolo el elemento2",heap_encolar(heap,elemento2)== true);
	print_test("Se encolo el elemento3",heap_encolar(heap,elemento3)== true);
	print_test("Se encolo el elemento4",heap_encolar(heap,elemento4)== true);
	print_test("El maximo del heap es elemento1",heap_ver_max(heap) == elemento1);
	print_test("Desencolo",heap_desencolar(heap) == elemento1);
	print_test("El maximo del heap es elemento2",heap_ver_max(heap) == elemento2);
	print_test("Desencolo",heap_desencolar(heap) == elemento2);
	print_test("El maximo del heap es elemento3",heap_ver_max(heap) == elemento3);
	print_test("Desencolo",heap_desencolar(heap) == elemento3);
	print_test("El maximo del heap es elemento4",heap_ver_max(heap) == elemento4);
	print_test("Desencolo",heap_desencolar(heap) == elemento4);
	print_test("El maximo del heap es NULL",heap_ver_max(heap) == NULL);
	print_test("El heap está vacío",heap_esta_vacio(heap) == true);
	print_test("No puedo desencolar en un heap vacío",heap_desencolar(heap) == NULL);
	print_test("No puedo ver el maximo de un heap vacío",heap_ver_max(heap) == NULL);
	heap_destruir(heap,NULL);
	print_test("El heap fue destruido correctamente",true);
}

void prueba_volumen (){
	heap_t* heap = heap_crear(cmp_func);
	int j;
	int* vector = malloc(sizeof(int) * 10000);
	for (int i = 0; i != 10000 ;i++){
		vector[i] = i;
		heap_encolar(heap,&vector[i]);
	}
	print_test("La cantidad en el heap es 1000",heap_cantidad(heap) == 10000);
	for (j = 9999; j >= 0 ;j--){
		bool desencolo_bien = *(int*)heap_desencolar(heap) == j;
		if (desencolo_bien == false){
			print_test("No se pudo desencolar correctamente", false);
		}
		bool comprobar_largo = heap_cantidad(heap) == j;
		if(comprobar_largo == false){
			print_test("Fallo la cantidad del heap", false);
		}
	}
	print_test("Encolar y desencolar 10000 elementos",heap_esta_vacio(heap));
	heap_destruir(heap,NULL);
	free(vector);
}

int _strcmp(const void* a,const void* b){
	return strcmp((char*) a,(char*) b);
}

void prueba_volumen_iguales(){
	heap_t* heap = heap_crear(_strcmp);
	char* cadenas[] = {"aaaa","abab","acac","adad","afaf","agag","amam","axax","azaz"};
	for (int i = 0; i < 800;i++){
		heap_encolar(heap,cadenas[i%8]);
	}
	int contar = 799;
	while(!heap_esta_vacio(heap)){

		int actual = 0;
		if (contar >= 700) actual = 7;
		else if (contar >= 600) actual = 6;
		else if (contar >= 500) actual = 5;
		else if (contar >= 400) actual = 4;
		else if (contar >= 300) actual = 3;
		else if (contar >= 200) actual = 2;
		else if (contar >= 100) actual = 1;
		bool desencolo_bien = (char*)heap_desencolar(heap) == cadenas[actual];
		if (desencolo_bien == false){
			print_test("No se pudo desencolar correctamente", false);
		}
		contar--;
	}
	heap_destruir(heap,NULL);
}
void prueba_destruccion_heap_NULL() {
	heap_t* heap = heap_crear(cmp_func);
	int valor1 = 1, valor2 = 2, valor3 = 3;
	int* elemento1 = &valor1;
	int* elemento2 = &valor2;
	int* elemento3 = &valor3;
	print_test("Se encolo el elemento1",heap_encolar(heap,elemento1) == true);
	print_test("Se encolo el elemento2",heap_encolar(heap,elemento2) == true);
	print_test("Se encolo el elemento3",heap_encolar(heap,elemento3) == true);
	heap_destruir(heap,NULL);
	print_test("El heap ha sido destruido correctamente",true);
}

void prueba_destruccion_heap_free() {
	heap_t* heap = heap_crear(cmp_func);
	int* elemento1 = malloc(sizeof(int));
	int* elemento2 = malloc(sizeof(int));
	int* elemento3 = malloc(sizeof(int));
	int* elemento4 = malloc(sizeof(int));
	int* elemento5 = malloc(sizeof(int));
	int* elemento6 = malloc(sizeof(int));
	*elemento1 = 1, *elemento2 = 2, *elemento3 = 4, *elemento4 = 4, *elemento5 = 5, *elemento6 = 6;
	print_test("Se encolo el elemento2",heap_encolar(heap,elemento2) == true);
	print_test("Se encolo el elemento3",heap_encolar(heap,elemento3) == true);
	print_test("Se encolo el elemento5",heap_encolar(heap,elemento5) == true);
	print_test("Se encolo el elemento6",heap_encolar(heap,elemento6) == true);
	print_test("Se encolo el elemento1",heap_encolar(heap,elemento1) == true);
	print_test("Se encolo el elemento4",heap_encolar(heap,elemento4) == true);
	print_test("El máximo es 6",*(int*)heap_ver_max(heap) == 6);
	heap_destruir(heap,free);
	print_test("El heap fue destruido correctamente",true);
}

void prueba_crear_arr(){
	void** arr = arreglo();
	heap_t* elem_a_borrar = heap_crear(cmp_func);
	for(size_t i = 0; i < 5; i++){
		heap_encolar(elem_a_borrar,arr[i]);
	}
	heap_t* heap = heap_crear_arr(arr,5,cmp_func);
	print_test("El max del heap es 20",*(int*)heap_ver_max(heap) == 20);
	print_test("Desencolo",*(int*)heap_desencolar(heap) == 20);
	print_test("El max del heap es 9",*(int*)heap_ver_max(heap) == 9);
	print_test("Desencolo",*(int*)heap_desencolar(heap) == 9);
	print_test("El max del heap es 4",*(int*)heap_ver_max(heap) == 4);
	print_test("Desencolo",*(int*)heap_desencolar(heap) == 4);
	print_test("El max del heap es 2",*(int*)heap_ver_max(heap) == 2);
	print_test("Desencolo",*(int*)heap_desencolar(heap) == 2);
	print_test("El max del heap es 1",*(int*)heap_ver_max(heap) == 1);
	print_test("Desencolo",*(int*)heap_desencolar(heap) == 1);
	print_test("El max del heap es NULL",heap_ver_max(heap) == NULL);
	heap_destruir(elem_a_borrar,free);
	heap_destruir(heap,NULL);
	print_test("El heap se destruyo correctamente",true);
	free(arr);
}

void prueba_heapsort(){
	void** arr = arreglo();
	bool esta_bien = true;
	heap_sort(arr ,5 ,cmp_func);
	for(int i = 0; i < 4 ; i ++){
		if(*(int*)arr[i] > *(int*)arr[i+1]){
			esta_bien = false;
			break;
		}
	}
	print_test("Se ordeno bien",esta_bien);
	for(int j = 0 ; j < 5; j++ ){
		free(arr[j]);
	}
	free(arr);
}

void pruebas_heap_alumno(){
	pruebas_heap_vacio();
	pruebas_comportamiento_heap();
	prueba_volumen();
	prueba_destruccion_heap_NULL();
	prueba_destruccion_heap_free();
	prueba_crear_arr();
	prueba_heapsort();
	prueba_volumen_iguales();
}
