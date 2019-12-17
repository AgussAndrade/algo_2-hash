import csv
import sys
import random
from grafo import*
from pilacola import*
from funciones_grafo import *
from funciones_tp import*
PARAMETROS_COMANDOS = sys.stdin
PARAMETROS = sys.argv[1:]
LARGO_PARAMETROS = len(PARAMETROS)
OPERACIONES = {'camino_mas' :0,'camino_escalas':1,'centralidad_aprox':3,'centralidad':2,'pagerank':4,'nueva_aerolinea':5,'recorrer_mundo':6,'recorrer_mundo_aprox':7,'vacaciones':8,'itinerario':9,'exportar_kml':10}
NO_PERTENECE_OPERACIONES = 'NO_PERTENECE_OPERACIONES'
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
def revisar_parametros():
	if LARGO_PARAMETROS != 2:
		print(ERROR_PARAMETROS)
		return False
	if(PARAMETROS[0].find('.csv') != -1 and PARAMETROS[1].find('.csv') != -1):
		return True
	return False
def guardar_aeropuertos():
	try:
		with open(PARAMETROS[0],encoding = 'utf8') as archivo_aeropuertos:
			r = csv.reader(archivo_aeropuertos,delimiter = ',')
			aeropuertos ={}
			linea = next(r,None)
			while linea:
				if(linea[0] in aeropuertos): aeropuertos[linea[0]].append(linea)
				else:
					aeropuertos[linea[0]] = [linea]
				linea = next(r,None)
			return aeropuertos
	except FileNotFoundError:
		print(ER_CARPETA)
	return None
def guardar_vuelos():
	try:
		with open(PARAMETROS[1],encoding = 'utf8') as archivo_vuelos:
			r = csv.reader(archivo_vuelos,delimiter = ',')
			vuelos = Grafo()
			linea = next(r,None)
			while linea:
				vuelos.agregar_arista(linea[0],linea[1],linea[2:])
				vuelos.agregar_arista(linea[1],linea[0],linea[2:])
				linea = next(r,None)
			return vuelos
	except FileNotFoundError:
		print(ER_CARPETA)
	return None
def envolver_comandos(aeropuertos,vuelos):
	for linea in PARAMETROS_COMANDOS:
		comando = linea.rstrip('\n').split(' ')
		if comando[0] == 'listar_operaciones':
			for key in OPERACIONES.keys():
				print(f'{key}')
			continue
		if not comando[0] in OPERACIONES:
			print(NO_PERTENECE_OPERACIONES)
			continue
		valor = OPERACIONES[comando[0]]
		comandos_validos = (' '.join(comando[1:])).split(',')
		largo = len(comandos_validos)
		if valor == 0:
			if(comandos_validos[0] == 'rapido'):
				camino_mas(0,aeropuertos,vuelos,comandos_validos[1],comandos_validos[2])
			if(comandos_validos[0] == 'barato'):
				camino_mas(1,aeropuertos,vuelos,comandos_validos[1],comandos_validos[2])
		elif valor == 1:
			camino_escalas(aeropuertos,vuelos,comandos_validos[0],comandos_validos[1])
		elif valor== 2:
			centralidad(vuelos,int(comandos_validos[0]))
		elif valor==3:
			centralidad_aprox(vuelos,int(comandos_validos[0]),300,10)
		elif valor ==4:
			pagerank(nueva_aerolinea(aeropuertos,vuelos,comandos_validos[0]))
		elif valor==5:
			nueva_aerolinea(aeropuertos,vuelos,comandos_validos[0])
		elif valor == 6:
			recorrer_mundo(aeropuertos,vuelos,comandos_validos[0])
		elif valor == 7:
			recorrer_mundo_aprox(aeropuertos,vuelos,comandos_validos[0],int(comandos_validos[1]))
		if valor == 8:
			vacaciones(aeropuertos,vuelos,comandos_validos[0],int(comandos_validos[1]))

def main():
	if revisar_parametros():
		aeropuertos = guardar_aeropuertos()
		vuelos = guardar_vuelos()
		envolver_comandos(aeropuertos,vuelos)

main()