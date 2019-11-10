#include "heap.h"
#include "testing.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/* Prototipo de función de comparación que se le pasa como parámetro a las
 * diversas funciones del heap.
 * Debe recibir dos punteros del tipo de dato utilizado en el heap, y
 * debe devolver:
 *   menor a 0  si  a < b
 *       0      si  a == b
 *   mayor a 0  si  a > b
 */
int cmp_func(const void *a, const void *b){
	if (*(int*)a < *(int*)b) return -1;
	if (*(int*)a == *(int*)b) return 0;
	return 1;
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
	int* vector = malloc(sizeof(int) * 1000);
	for (int i = 0; i != 1000 ;i++){
		vector[i] = i;
		heap_encolar(heap,&vector[i]);
	}
	print_test("La cantidad en el heap es 1000",heap_cantidad(heap) == 1000);
	for (j = 999; j >= 0 ;j--){
		bool desencolo_bien = *(int*)heap_desencolar(heap) == j;
		if (desencolo_bien == false){
			print_test("No se pudo desencolar correctamente", false);
		}
		bool comprobar_largo = heap_cantidad(heap) == j;
		if(comprobar_largo == false){
			print_test("Fallo la cantidad del heap", false);
		}
	}
	print_test("Encolar y desencolar 1000 elementos",heap_esta_vacio(heap));
	heap_destruir(heap,NULL);
	free(vector);
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
	*elemento1 = 1, *elemento2 = 2, *elemento3 = 3;
	print_test("Se encolo el elemento1",heap_encolar(heap,elemento1) == true);
	print_test("Se encolo el elemento2",heap_encolar(heap,elemento2) == true);
	print_test("Se encolo el elemento3",heap_encolar(heap,elemento3) == true);
	heap_destruir(heap,free);
	print_test("El heap fue destruido correctamente",true);
}
void pruebas_heap_alumno(){
	pruebas_heap_vacio();
	pruebas_comportamiento_heap();
	prueba_volumen();
	prueba_destruccion_heap_NULL();
	prueba_destruccion_heap_free();

}
