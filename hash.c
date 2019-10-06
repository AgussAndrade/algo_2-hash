#include "hash.h"

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>

#define LARGO_I 31

typedef struct campo{
	char* clave;
	void* valor;
	bool estado; //false ocupado, true borrado. Si esta vacio es porque no hay campo por ende tampoco estado xdxdxd LOLARDO
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
		if(aux[i] != NULL){
			if(aux[i]->estado){
				free(aux[i]);
			}
			else{
				hash_guardar(aux[i]->clave,aux[i]->valor);
				free(aux[i]);
			}
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
        hash = 31*hash + word[i];
    }
    return hash % SIZE;
}

int hash_buscar(hash_t* hash,char* clave){
	int pos = hashing(clave);
	int i = pos;
	campo_t campo = NULL;
	while (true){
		if(hash->arr[i] == NULL){
			return i;
		}
		else{
			if (hash->arr[i]->clave == clave){
				return i;
			}
		}
		if( i == hash->capacidad - 1){
			i = 0;
			continue;
		}
		i++;
		if(i == pos){
			break;
		}
	}
	return -1;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	hash->capacidad = LARGO_I;
	hash->cantidad = 0;
	hash->hash_destruir_dato_t = destruir_dato;
	campo_t* arreglo = malloc(sizeof(campo_t)*LARGO_I);
	if (!arreglo){
		free(hash);
		return NULL;
	}
	for(int i = 0; i< LARGO_I;i++){
		arreglo[i]= NULL;
	}
	hash->arr = arreglo;
	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	if((hash->cantidad + 1) *10 / hash->capacidad >= 7){
		if(!hash_redimensionar(hash)) return false;
	}
	int pos = hash_buscar(hash,clave);

	if(hash->arr[pos] == NULL){
		campo_t* campo = malloc(sizeof(campo_t));
		if(!campo) return false;
		campo->clave = clave;
		campo->dato = dato;
		campo->estado = false;
	}
	else{
		hash->destruir(hash->arr[pos]->valor);
		hash->arr[pos]->valor = dato;
		hash->arr[pos]->estado = false;
	}
	hash->cantidad++;
	return true;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	int pos = hash_buscar(hash,clave);
	if(hash->arr[pos] == NULL){
		return NULL;
	}
	else if (hash->arr[pos]->estado){
		return NULL;
	}
	return hash->arr[pos]->valor;
}
void *hash_obtener(const hash_t *hash, const char *clave){
	if (hash_obtener(hash,clave)!= NULL){
		return true;
	}
	return false;
}


