class _Nodo:
	def __init__(self,dato = None,prox = None):
		self.dato = dato
		self.prox = prox

class Cola:
	def __init__(self):
		self.prim = None
		self.ultimo = None
	def ver_primero(self):
		return self.prim.dato
	def encolar(self,dato):
		nuevo_nodo = _Nodo(dato)
		if self.ultimo is not None:
			self.utimo.prox = nuevo_nodo
			self.ultimo = nuevo_nodo
		else:
			self.prim = nuevo_nodo
			self.ultimo = nuevo_nodo
	def desencolar(self):
		if self.esta_vacia():
			return None
		dato = self.prim.dato
		self.prim = self.prim.prox
		if not self.prim:
			self.ultimo = None
		return dato
	def esta_vacia(self):
		return self.ultimo == None

class Pila:
	def __init__(self):
		self.ult = None
	def apilar(self,dato):
		if self.ult == None:
			self.ult = _Nodo(dato)
		else:
			dato_a_agregar = _Nodo(dato,self.ult)
			self.ult = dato_a_agregar
	def	desapilar(self):
		dato = self.ult.dato
		self.ult = self.ult.prox
	def esta_vacia(self):
		return self.ult == None

def obtener_padre(pos):
	return (pos -1 )//2
def ob_hijo_i(pos):
	return 2*pos + 1
def ob_hijo_d(pos):
	return 2*pos+2
def swap(arr,i,j):
	aux = arr[i]
	arr[i] = arr[j]
	arr[j] = aux
def upheap(arr,inicio,act,final):
	if act <= inicio or act >= final: return
	i = act
	while i > inicio:
		padre = obtener_padre(i)
		if arr[i][0] < arr[padre][0]:
			swap(arr,i,padre)
			i = padre
		else: return
	return
def downheap(arr,inicio,act,final):
	if act< inicio or act >= final : return
	i = act
	while i < final:
		hijo_d = ob_hijo_d(i)
		hijo_i = ob_hijo_i(i)
		if(hijo_d < final and hijo_i < final):
			aux = 0
			if(arr[hijo_d][0] < arr[hijo_i][0]):
				aux = hijo_d
			else: aux = hijo_i
			if arr[aux][0] < arr[i][0]:
				swap(arr,i,aux)
				i = aux
				continue
			else: return
		if hijo_d < final:
			if arr[hijo_d][0] < arr[i][0]:
				swap(arr,i,hijo_d)
				i = hijo_d
				continue
			return
		elif hijo_i < final:
			if arr[hijo_i][0] < arr[i][0]:
				swap(arr,i,hijo_i)
				i = hijo_i
				continue
		else: return
	return
class Heap:
	def __init__(self):
		self.arr = []
		self.cant = 0
	def __len__(self):
		return self.cant
	def ver_min(self):
		return self.arr[0]
	def esta_vacio(self):
		return self.cant == 0
	def desencolar(self):
		if self.esta_vacio(): return None
		elem = self.arr[0]
		swap(self.arr,0,self.cant -1)
		self.arr.pop(self.cantidad -1)
		self.cant -=1
		downheap(self.arr,0,0,self.cant)
		return elem
	def encolar(self,elem):
		self.arr.append(elem)
		upheap(self.arr,0,self.cant,self.cant +1)
		self.cant +=1

def quicksort(lista):
	if len(lista)<2:
		return lista
	i,m,d = partir(lista,len(lista)-1)
	return quicksort(i) + [m] + quicksort(d)
def partir(lista,fin):
	pivote = lista[0]
	i = []
	d = []
	i =1
	while i <= fin:
		if lista[i][1] < pivote[1]:
			i.append(lista[i])
			i +=1
		else:
			d.append(lista[i])
			i +=1
	return i,pivote,d
def merge_sort(lista):
	if len(lista) <2:
		return lista
	medio = len(lista)//2
	izq = merge_sort(lista[:medio])
	der = merge_sort(lista[medio:])
	return merge(izq,der)

def merge(l1,l2):
	i,j=0,0
	resultado =[]
	while(i<len(l1)and j<len(l2)):
		if(l1[i][1] > l2[i][1]):
			resultado.append(l1[i])
			i+=1
		else:
			resultado.append(l2[i])
			j+=1
	resultado += l1[i:]
	resultado+= l2[j:]
	return resultado