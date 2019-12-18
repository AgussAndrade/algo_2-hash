from grafo import*
from pilacola import*

def bfs(grafo,origen):
	visitados = set()
	padres = {}
	orden = {}
	q = Cola()
	visitados.add(origen)
	padres[origen] = None
	orden[origen] = 0
	q.encolar(origen)
	while not q.esta_vacia():
		v = q.desencolar()
		for w in grafo.adyacentes(v):
			if w not in visitados:
				visitados.add(w)
				q.encolar(w)
				padres[w] = v
				orden[w] = orden[v] + 1
	return padres,orden
def orden_topologico(grafo):
	grados = {}
	vertices = grafo.obtener_vertices()
	for v in vertices: grados[v] = 0
	for v in vertices:
		for w in grafo.adyacentes(v):
			grados[w] +=1
	q = Cola()
	for v in vertices:
		if grados[v] == 0: q.encolar(v)
	resul = []
	while not q.esta_vacia():
		v = q.desencolar()
		resul.append(v)
		for w in grafo.adyacentes(v):
			grados[w] -=1
			if grados[w] == 0:
				q.encolar(w)
	if(len(resul) == len(grafo)): return resul
	return None
def camino_minimo(grafo,origen,i = None,destino = None):
	dist = {}
	padres = {}
	for v in grafo.obtener_vertices():
		dist[v] = float('inf')
	dist[origen] = 0
	padres[origen] = None
	q = Heap()
	q.encolar([dist[origen],origen])
	while not q.esta_vacio():
		distancia,v = q.desencolar()
		if destino is not None:
			if destino == v:
				return padres,dist
		for w in grafo.adyacentes(v):
			peso = grafo.ver_peso(v,w)
			if i != None:
				peso = int(peso[i])
			else: peso = 1
			if dist[v] + peso < dist[w]:
				dist[w] = dist[v] + peso
				padres[w] = v
				q.encolar([dist[w],w])
	return padres,dist

def ordenar_vertices(grafo,distancia):
	vertices = grafo.obtener_vertices()
	rta = []
	for v in vertices:
		if distancia[v] != 0:
			rta.append((v,distancia[v]))
	rta_2 = quicksort(rta)
	i = 0
	j = len(rta) -1
	while i < len(rta):
		rta[i] = rta_2[j]
		j-=1
		i+=1
	return rta
def camino_minimo_centralidad(grafo,origen,i = None,destino = None):
	dist = {}
	padres = {}
	for v in grafo.obtener_vertices():
		dist[v] = float('inf')
	dist[origen] = 0
	padres[origen] = None
	q = Heap()
	q.encolar([dist[origen],origen])
	while not q.esta_vacio():
		distancia,v = q.desencolar()
		if destino is not None:
			if destino == v:
				return padres,dist
		for w in grafo.adyacentes(v):
			peso = grafo.ver_peso(v,w)
			if i != None:
				peso = int(peso[i])
			else: peso = 1
			if dist[v] + 1/peso < dist[w]:
				dist[w] = dist[v] + 1/peso
				padres[w] = v
				q.encolar([dist[w],w])
	return padres,dist
def grafo_centralidad(grafo,formato = None):
	cent = {}
	vertices = grafo.obtener_vertices()
	for v in vertices: cent[v] = 0
	for v in vertices:
		padre,distancia = camino_minimo_centralidad(grafo,v,formato)
		cent_aux = {}
		for w in vertices: cent_aux[w] = 0
		vertices_ordenados = ordenar_vertices(grafo,distancia)
		for w,d in vertices_ordenados:
			cent_aux[padre[w]] += 1 + cent_aux[w]
		for w in vertices:
			if w == v: continue
			cent[w] += cent_aux[w]
	return cent

def mst_prim(grafo,vertice = None,parametro_peso = None):
	if vertice == None: vertice = grafo.obtener_vertice_azar()
	visitados = set()
	visitados.add(vertice)
	q = Heap()
	for w in grafo.adyacentes(vertice):
		peso = grafo.ver_peso(vertice,w)
		if parametro_peso:
			peso = int(peso[parametro_peso])
		q.encolar((peso,vertice,w))
	arbol = Grafo()
	while not q.esta_vacio():
		p,v,w = q.desencolar()
		if w in visitados: continue
		arbol.agregar_arista(v,w,p)
		visitados.add(w)
		for x in grafo.adyacentes(w):
			if x not in visitados:
				peso = grafo.ver_peso(w,x)
				if parametro_peso:
					peso = int(peso[parametro_peso])
				q.encolar((peso,w,x))
	return arbol
