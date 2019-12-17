import csv
import sys
import random
from grafo import*
from pilacola import*
from funciones_grafo import *
ER_CARPETA = 'ER_CARPETA'
ERROR_CENTRALIDAD_APROX = 'ERROR_CENTRALIDAD_APROX'
ERROR_VACACIONES = 'ERROR_VACACIONES'
ERROR_PARAMETROS = 'ERROR_PARAMETROS'
ERROR_CENTRALIDAD = 'ERROR_CENTRALIDAD'
ERROR_CAMINO_MAS = 'ERROR_CAMINO_MAS'
ERROR_NUEVA_AEROLINEA = 'ERROR_NUEVA_AEROLINEA'
ERROR_RECORRER_MUNDO = 'ERROR_RECORRER_MUNDO'
ERROR_RECORRER_MUNDO_APROX = 'ERROR_RECORRER_MUNDO_APROX'
ERROR_PAGERANK = 'ERROR_PAGERANK'
ERROR_CAMINO_ESCALAS = 'ERROR_CAMINO_ESCALAS'

def camino_mas(formato,aeropuertos,vuelos,desde,hasta):
	if not desde in aeropuertos or not hasta in aeropuertos:
		print(ERROR_CAMINO_MAS)
		return
	codigos_validos = []
	padres ={}
	dist = {}
	menos = float('inf')
	a_utilizar = ''
	for datos_hasta in aeropuertos[hasta]:
		codigos_validos.append(datos_hasta[1])
	for listas in aeropuertos[desde]:
		padres_aux,dist_aux = camino_minimo(vuelos,listas[1],formato)
		for aeropuerto_hasta in codigos_validos:
			if dist_aux[aeropuerto_hasta] < menor:
				menor = dist_aux[aeropuerto_hasta]
				dist = dist_aux
				padres = padres_aux
	pila = Pila()
	while a_utilizar != None:
		pila.apilar(a_utilizar)
		a_utilizar = padres[a_utilizar]
	while not pila.esta_vacia():
		print(pila.desapilar(),end = '')
		if not pila.esta_vacia():
			print(' -> ',end = '')
	print()
	return
def camino_escalas(aeropuertos,vuelos,desde,hasta):
	if not desde in aeropuertos or not hasta in aeropuertos:
		print(ERROR_CAMINO_ESCALAS)
		return
	codigos_validos = []
	padres = {}
	orden = {}
	menos = float('inf')
	a_utilizar = ''
	for datos_hasta in aeropuertos[hasta]:
		codigos_validos.append(datos_hasta[1])
	for listas in aeropuertos[desde]:
		padres_aux,dist_aux = bfs(vuelos,listas[1])
		for aeropuerto_hasta in codigos_validos:
			if dist_aux[aeropuerto_hasta] < menor:
				menor = dist_aux[aeropuerto_hasta]
				dist = dist_aux
				padres = padres_aux
	pila = Pila()
	while a_utilizar != None:
		pila.apilar(a_utilizar)
		a_utilizar = padres[a_utilizar]
	while not pila.esta_vacia():
		print(pila.desapilar(),end = '')
		if not pila.esta_vacia():
			print(' -> ',end = '')
	print()
	return
def centralidad (vuelos,formato):
	cent = grafo_centralidad(vuelos)
	minimos = Heap()
	contador = 0
	for codigo,tam in cent.items():
		if(contador<formato):
			minimos.encolar((tam,codigo))
			contador+=1
			continue
		if tam > minimos.ver_min()[0]:
			minimos.desencolar()
			minimos.encolar((tam,codigo))
		pila = Pila()
		while not minimos.esta_vacio():
			pila.apilar(minimos.desencolar()[1])
		while not pila.esta_vacia():
			print(pila.desapilar(),end = '')
			if not pila.esta_vacia():
				print(', ',end = '')
		print()
def centralidad_aprox(grafo,formato,largo,recorridos):
	heap = Heap()
	apariciones = {}
	adyacentes = {}
	vertices = []
	contador = 0
	for v in grafo.obtener_vertices():
		vertices.append(v)
		apariciones[v] = 0
		valores = {}
		for w in grafo.adyacentes(v):
			valores[w] = int(grafo.obtener_arista(v,w)[2])
			adyacentes[v] = valores
	for _ in range(recorridos):
		origen = random.choice(vertices)
		apariciones[origen] +=1
		for _ in range(largo):
			actual = " ".join(random.choices(list(adyacentes[origen].keys()),list(adyacentes[origen].values())))
			apariciones[actual] +=1
			origen = actual
	for k,v in apariciones.items():
		if(contador < formato):
			heap.encolar((v,k))
			contador +=1
			continue
		if v>heap.ver_min()[0]:
			heap.desencolar()
			heap.encolar((v,k))
	pila = Pila()
	while not heap.esta_vacio():
		pila.apilar(heap.desencolar()[1])
	while not pila.esta_vacia():
		print(pila.desencolar(),end  = '')
		if not pila.esta_vacia():
			print(', ',end = '')
	print()
	return
def pagerank(grafo,cantidad,iteraciones):
	pagerank = {}
	links = {}
	cant_vertices = 0
	contador = 0
	rank = Heap()
	vertices = grafo.obtener_vertices()
	for v in vertices:
		cant_vertices +=1
		pagerank[v] = 0
		links[v] = 0
		for w in grafo.adyacentes(v):
			links[v] +=1
	for i in range(iteraciones):
		for vert in vertices:
			pagerank_suma = 0
			for w in grafo.adyacentes(vert):
				pagerank_suma +=(pagerank[w]/links[w])
			pagerank[vert] +=(((1-0.85)/cant_vertices) + (0.85 * pagerank_suma))
	for k,v in pagerank.items():
		if(contador<cantidad):
			rank.encolar((v,k))
			contador +=1
			continue
		if v > rank.ver_min()[0]:
			rank.desencolar()
			rank.encolar((v,k))
	pila = Pila()
	while not rank.esta_vacio():
		pila.apilar(rank.desencolar()[1])
	while not pila.esta_vacia():
		print(pila.desapilar(),end = '')
		if not pila.esta_vacia():
			print(', ',end = '')
	print()
def nueva_aerolinea(aeropuertos,vuelos,archivo_a_escribir):
	try:
		with open(archivo_a_escribir,'w',encoding = 'utf8') as archivo:
			arbol_a_escribir = mst_prim(vuelos,None,0)
			visitados = set()
			for v in arbol_a_escribir.obtener_vertices():
				for w in arbol_a_escribir.adyacentes():
					if not (v,w) in visitados and(w,v) not in visitados:
						visitados.add((v,w))
						arr_a_escribir = [v,w] + vuelos.ver_peso(v,w)
						linea = ','.join(arr_a_escribir)
						archivo.write(f'{linea}\n')
		print('OK')
	except FileNotFoundError:
		print(ERROR_NUEVA_AEROLINEA)
def _vacaciones(vuelos,desde,actual,rta,visitados,cant_visitados,cantidad):
	if cant_visitados == cantidad:
		if vuelos.estan_unidos(desde,actual): return True
		else: return False
	ady = vuelos.adyacentes(actual)
	for v in ady:
		if v not in visitados:
			rta.append(v)
			visitados.add(v)
			cant_visitados +=1
			if(_vacaciones(vuelos,desde,v,rta,visitados,cant_visitados,cantidad)):
				return True
			rta.pop()
			visitados.remove(v)
			cant_visitados -=1
def vacaciones(aeropuertos,vuelos,desde,cantidad):
	if not desde in aeropuertos:
		print(ERROR_VACACIONES)
	rta = []
	visitados = set()
	cant_visitados = 0
	for codigos in aeropuertos[desde]:
		codigo = codigos[1]
		rta.append(codigo)
		visitados.add(codigo)
		cant_visitados +=1
		if(_vacaciones(vuelos,codigo,codigo,rta,visitados,cant_visitados,cantidad)):
			largo_rta = len(rta)
			for i in range(largo_rta):
				print(f'{rta[i]} -> ',end = '')
				print(rta[0])
				return
		else:
			rta.pop()
			visitados.remove(codigo)
			cant_visitados -=1
	print('No existe un vuelo')
	return
def _recorrer_mundo_aprox(vuelos,actual,rta,valor,visitados,cant_visitados):
	if cant_visitados == len(vuelos):
		return True
	if len(rta) > 1000:
		return False
	padres,dist = camino_minimo(vuelos,actual,0)
	tiempo_min = []
	for v,distancia in dist.items():
		tiempo_min.append((v,distancia))
	tiempo_ordenado = quicksort(tiempo_min)
	for tupla in tiempo_ordenado:
		v = tupla[0]
		if v not in visitados:
			valor[0] += tupla[1]
			w = padres[v]
			aniadidos = []
			pila = Pila()
			while w != None:
				if w != actual:
					pila.apilar(w)
					if w not in visitados:
						cant_visitados +=1
						aniadidos.append(w)
						visitados.add(w)
				w = padres[w]
			while not pila.esta_vacia():
				rta.append(pila.desapilar())
			visitados.add(v)
			rta.append(v)
			cant_visitados +=1
			if(_recorrer_mundo_aprox(vuelos,v,rta,valor,visitados,cant_visitados)):
				return True
			w = padres[v]
			while w != None:
				if w != actual:
					rta.pop()
					w = padres[w]
				for t in aniadidos:
					visitados.remove(t)
					cant_visitados -=1
			rta.pop()
			visitados.remove(v)
			cant_visitados -=1
			valor[0] -= distancia
	return False
def recorrer_mundo_aprox(aeropuertos,vuelos,desde):
	if not desde in aeropuertos:
		print(ERROR_RECORRER_MUNDO_APROX)
		return
	rta = []
	valor = [0]
	visitado = set()
	cant_visitados = 0
	for codigos in aeropuertos[desde]:
		codigo = codigos[1]
		visitados.add(codigo)
		cant_visitados +=1
		if(_recorrer_mundo_aprox(vuelos,codigo,rta,valor,visitados,cant_visitados)):
			print(f'{codigo} -> ',end='')
			largo = len(rta)
			for i in range(largo):
				print(f'{rta[i]} ',end = '')
				if i < largo -1:
					print('-> ',end = '')
			print()
			print(valor[0])
			return
		else:
			cant_visitados -=1
			visitados.remove(codigo)
	print('no se encontro camino')
	return