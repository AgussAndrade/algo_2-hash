#include "hash.h"
#undef get16bits

#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif // cosas del hash 

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define _POSIX_C_SOURCE
#define LARGO_I 37
#define MAX_C 7
#define MIN_C 1 


typedef enum {vacio,ocupado,borrado} estado_t;

typedef struct campo{
	const char* clave;
	void* valor;
	estado_t estado;
} campo_t;

struct hash{
	size_t capacidad;
	size_t cantidad;
	size_t ocupado;
	campo_t* arr;
	hash_destruir_dato_t destruir;
};

struct hash_iter{
	const hash_t* hash;
	size_t pos;
};

char *copiar_cadena(const char *str){
	size_t j = strlen(str);

	char* nueva_cadena = malloc(sizeof(char) * (j+1));
	if (nueva_cadena == NULL){
		return NULL;
	}
	if( strcmp(str,"") == 0){
		nueva_cadena[0] = '\0';
		return nueva_cadena;
	}
	strcpy(nueva_cadena,str);
	nueva_cadena[j]= '\0';
	return nueva_cadena;
}


size_t hashing (const char * data, size_t len) {
size_t hash = len, tmp;
size_t rem;

    if (len <= 0 || data == NULL) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--) {
        hash  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        data  += 2*sizeof (uint16_t);
        hash  += hash >> 11;
    }

    /* Handle end cases */
    switch (rem) {
        case 3: hash += get16bits (data);
                hash ^= hash << 16;
                hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
                hash += hash >> 11;
                break;
        case 2: hash += get16bits (data);
                hash ^= hash << 11;
                hash += hash >> 17;
                break;
        case 1: hash += (signed char)*data;
                hash ^= hash << 10;
                hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}



bool hash_redimensionar (hash_t* hash,size_t tam){
	campo_t* datos_nuevo = calloc(3,tam * sizeof(campo_t));
	if (datos_nuevo == NULL){
		return false;
	}

	campo_t* aux = hash->arr;
	hash->arr = datos_nuevo;
	size_t j = hash->capacidad;
	hash->capacidad = tam;
	hash->cantidad = 0;
	hash->ocupado = 0;
	for(int i = 0; i < j;i++){
		
		if(aux[i].estado == ocupado){
			hash_guardar(hash,aux[i].clave,aux[i].valor);
			char* a_liberar = (char*)aux[i].clave;
			free(a_liberar);
		}
	}
	free(aux);
	return true;

}

size_t hash_buscar(const hash_t* hash,const char* clave){
	size_t pos = hashing(clave, strlen(clave)) % hash->capacidad;
	size_t i = pos;
	while (true){
		// printf("%i %i %i\n",i,pos, hash->capacidad );
		if(hash->arr[i].estado == vacio){
			break;
		}
		else if (hash->arr[i].estado == ocupado && strcmp(hash->arr[i].clave, clave) == 0){
			break;
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
	return i; //es imposible que devuelva esto por el tema de que nunca se llena pero sin esto no compila
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	
	hash->capacidad = LARGO_I;
	hash->cantidad = 0;
	hash->destruir = destruir_dato;
	hash->ocupado = 0;
	campo_t* arreglo = calloc(3,sizeof(campo_t)*LARGO_I);
	
	if (!arreglo){
		free(hash);
		return NULL;
	}	
	hash->arr = arreglo;
	return hash;
}

bool hash_guardar( hash_t *hash, const char *clave, void *dato){
	if((hash->ocupado + 1) *10 / hash->capacidad >= MAX_C){
		if(!hash_redimensionar(hash,(hash->capacidad)*2 )) return false;
	}
	const hash_t* A = hash;
	size_t pos = hash_buscar(A,clave);

	if(hash->arr[pos].estado == vacio ){
		const char* cla_aux = copiar_cadena(clave);
		if( cla_aux == NULL){
			return NULL;
		}
		hash->arr[pos].clave = cla_aux;
		hash->arr[pos].valor = dato;
		hash->arr[pos].estado = ocupado;
		hash->cantidad++;
		hash->ocupado++;

	}
	else { 
		if(hash->destruir != NULL){
			hash->destruir(hash->arr[pos].valor); 
		}
		hash->arr[pos].valor = dato;
		hash->arr[pos].estado = ocupado;
	}
	return true;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	size_t pos = hash_buscar(hash,clave);
	if(hash->arr[pos].estado != ocupado){
		return NULL;
	}
	return hash->arr[pos].valor;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	size_t pos = hash_buscar(hash,clave);
	if (hash->arr[pos].estado == ocupado){
		return true;
	}
	return false;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cantidad;
}

void *hash_borrar(hash_t *hash, const char *clave){
	size_t pos = hash_buscar(hash,clave);
	// printf("%i %i\n",pos, hash->capacidad );
	if (hash->arr[pos].estado != ocupado){
		return NULL;
	}
	if ((hash->capacidad >= LARGO_I) && ((hash->ocupado)*10/hash->capacidad <= MIN_C)){
		hash_redimensionar(hash, hash->capacidad/2);
		pos = hash_buscar(hash,clave);
	}
	free((char*)hash->arr[pos].clave);
	hash->arr[pos].clave = NULL;
	hash->arr[pos].estado = borrado;
	void* rta = hash->arr[pos].valor;
	hash->arr[pos].valor = NULL;

	hash->cantidad --;
	
	return rta;
}
void hash_destruir(hash_t *hash){
	campo_t* arreglo = hash->arr;

	for(int i =0; i<hash->capacidad;i++){
		if (arreglo[i].estado == ocupado){
			if(hash->destruir != NULL) hash->destruir(arreglo[i].valor);
			char* a_liberar = (char*)arreglo[i].clave;
			// printf("%s\n","libero en hash_destruir" );

			free(a_liberar);
		} 
	}

	free(arreglo);
	free(hash);
}
// Crea iterador
hash_iter_t* hash_iter_crear(const hash_t* hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	
	if (iter == NULL){
		return NULL;
	}
	size_t pos = 0;
	iter->hash = hash;
	
	if(hash->cantidad == 0){
		iter->pos = hash->capacidad;
	}
	else{
		while(hash->arr[pos].estado != ocupado){
			pos ++;
		}
		iter->pos = pos;
	}
	return iter;
}

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t* iter){
	size_t pos = iter->pos + 1;
	if (hash_iter_al_final(iter)) return false;

	while( pos < iter->hash->capacidad){
		
		if( iter->hash->arr[pos].estado == ocupado){
			iter->pos = pos;
			return true;
		}
		
		pos++;
		
	}
	iter->pos = pos;
	return false;
}

const char* hash_iter_ver_actual(const hash_iter_t* iter){
	if(hash_iter_al_final(iter)){
		return NULL;
	}
	return iter->hash->arr[iter->pos].clave;
}

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t* iter){
	if (iter->pos >= iter->hash->capacidad){
		return true;
	}
	return false;
}

// Destruye iterador
void hash_iter_destruir(hash_iter_t* iter){
	free(iter);
}
