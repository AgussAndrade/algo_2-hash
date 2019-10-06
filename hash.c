#include "hash.h"

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>

#define LARGO_I 37

typedef enum {vacio,ocupado,borrado} estado_t;

typedef struct campo{
	const char* clave;
	void* valor;
	estado_t estado;
} campo_t;

struct hash{
	size_t capacidad;
	size_t cantidad;
	hash_destruir_dato_t* destruir;
	campo_t* arr;
};

struct hash_iter{
	hash_t* hash;
	campo_t* act;
	size_t pos; //posicion de donde estoy, si estoy en > a capacidad me pase y termine de iterar 
};

bool hash_redimensionar (hash_t* hash){
	campo_t* datos_nuevo = malloc(hash->capacidad * 2 * sizeof(campo_t));
	if (datos_nuevo == NULL){
		return false;
	}

	campo_t* aux = hash->arr;
	hash->arr = datos_nuevo;
	int j = hash->capacidad;
	hash->capacidad *=2;
	hash->cantidad = 0;

	for(int i = 0; i < j;i++){
		
		if(aux[i].estado == ocupado){
			hash_guardar(hash,aux[i].clave,aux[i].valor);
		}
	}
	free(aux);
	return true;

}

int hashing (const char* word)
{
    unsigned int hash = 0;
    for (int i = 0 ; word[i] != '\0' ; i++)
    {
        hash = 37*hash + word[i];
    }
    return hash;
}

int hash_buscar(const hash_t* hash,const char* clave){
	int pos = hashing(clave)%hash->capacidad;
	int i = pos;
	while (true){
		if(hash->arr[i].estado == vacio){
			return i;
		}
		else{
			if (hash->arr[i].clave == clave){
				return i;
			}
		}
		if( i >= hash->capacidad - 1){
			i = 0;
			continue;
		}
		i++;
		if(i == pos){
			break;
		}
	}
	return -1; //es imposible que devuelva -1 por el tema de que nunca se llena pero sin esto no compila
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	
	hash->capacidad = LARGO_I;
	hash->cantidad = 0;
	hash->destruir = destruir_dato; //ERROR
	
	campo_t* arreglo = malloc(sizeof(campo_t)*LARGO_I);
	
	if (!arreglo){
		free(hash);
		return NULL;
	}
	
	for(int i = 0 ; i < LARGO_I;i++){
		arreglo[i].estado = vacio;
	}
	
	hash->arr = arreglo;
	return hash;
}

bool hash_guardar( hash_t *hash, const char *clave, void *dato){
	if((hash->cantidad + 1) *10 / hash->capacidad >= 7){
		if(!hash_redimensionar(hash)) return false;
	}
	const hash_t* A = hash;
	int pos = hash_buscar(A,clave);

	if(hash->arr[pos].estado == vacio ){
//		campo_t* campo = malloc(sizeof(campo_t));
		hash->arr[pos].clave = clave;
		hash->arr[pos].valor = dato;
		hash->arr[pos].estado = ocupado;
	}
	else{
		hash->destruir(hash->arr[pos].valor); //ERROR
		hash->arr[pos].valor = dato;
		hash->arr[pos].estado = ocupado;
	}
	hash->cantidad++;
	return true;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	int pos = hash_buscar(hash,clave);
	if(hash->arr[pos].estado == vacio){
		return NULL;
	}
	else if (hash->arr[pos].estado == borrado){
		return NULL;
	}
	return hash->arr[pos].valor;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	int pos = hash_buscar(hash,clave);
	if (hash->arr[pos].estado == ocupado){
		return true;
	}
	return false;
}

// size_t hash_cantidad(const hash_t *hash){

// }
