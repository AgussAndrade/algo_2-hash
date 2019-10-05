#include "hash.h"

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

#define LARGO_I 31

typedef struct campo{
	char* clave;
	void* valor;
	bool estado; //false ocupado, true borrado. Si esta vacio es porque no hay campo por ende tampoco estado xdxdxd LOLARDO
} campo_t;

struct hash{
	size_t capacidad;
	size_t cantidad;
	hash_destruir_dato_t;
	campo_t* arr;
};

struct hash_iter{
	hash_t* hash;
	campo_t* act;
	size_t pos; //posicion de donde estoy, si estoy en > a capacidad me pase y termine de iterar 
}

// unsigned int hashing(const char *word) {
//     unsigned int hash = 0, c;
//     for (size_t i = 0; word[i] != '\0'; i++) {
//         c = (unsigned char)word[i];
//         hash = (hash << 3) + (hash >> (sizeof(hash) * CHAR_BIT - 3)) + c;
//     }
//     return hash % SIZE;
// }
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
