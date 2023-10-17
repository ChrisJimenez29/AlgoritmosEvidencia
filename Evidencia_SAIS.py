from collections import Counter
from memory_profiler import profile
import time

def cuenta_palabras(archivo):
    texto2 = ""
    with open(archivo, 'r', encoding='utf-8') as f:
        texto = f.read()
        palabras = texto.split()
        for palabra in palabras:
            texto2 += palabra
    return texto2

def getBuckets(T):
    count = {}
    buckets = {}
    for c in T:
        count[c] = count.get(c, 0) + 1
    start = 0
    for c in sorted(count.keys()):
        buckets[c] = (start, start + count[c])
        start += count[c]
    return buckets

def sais(T):
    t = ["_"] * len(T)

    t[len(T) - 1] = "S"
    for i in range(len(T) - 1, 0, -1):
        if T[i-1] == T[i]:
            t[i - 1] = t[i]
        else:
            t[i - 1] = "S" if T[i-1] < T[i] else "L"

    buckets = getBuckets(T)

    count = {}
    SA = [-1] * len(T)
    LMS = {}
    end = None
    for i in range(len(T) - 1, 0, -1):
        if t[i] == "S" and t[i - 1] == "L":
            revoffset = count[T[i]] = count.get(T[i], 0) + 1
            SA[buckets[T[i]][1] - revoffset] = i
            if end is not None:
                LMS[i] = end
            end = i

    LMS[len(T) - 1] = len(T) - 1
    count = {}
    for i in range(len(T)):
        if SA[i] >= 0:
            if t[SA[i] - 1] == "L":
                symbol = T[SA[i] - 1]
                offset = count.get(symbol, 0)
                SA[buckets[symbol][0] + offset] = SA[i] - 1
                count[symbol] = offset + 1

    count = {}
    for i in range(len(T) - 1, 0, -1):
        if SA[i] > 0:
            if t[SA[i] - 1] == "S":
                symbol = T[SA[i] - 1]
                revoffset = count[symbol] = count.get(symbol, 0) + 1
                SA[buckets[symbol][1] - revoffset] = SA[i] - 1

    namesp = [-1] * len(T)
    name = 0
    prev = None
    for i in range(len(SA)):
        if t[SA[i]] == "S" and t[SA[i] - 1] == "L":
            if prev is not None and T[SA[prev]:LMS[SA[prev]]] != T[SA[i]:LMS[SA[i]]]:
                name += 1
            prev = i
            namesp[SA[i]] = name

    names = []
    SApIdx = []
    for i in range(len(T)):
        if namesp[i] != -1:
            names.append(namesp[i])
            SApIdx.append(i)

    if name < len(names) - 1:
        names = sais(names)

    names = list(reversed(names))

    SA = [-1] * len(T)
    count = {}
    for i in range(len(names)):
        pos = SApIdx[names[i]]
        revoffset = count[T[pos]] = count.get(T[pos], 0) + 1
        SA[buckets[T[pos]][1] - revoffset] = pos

    count = {}
    for i in range(len(T)):
        if SA[i] >= 0:
            if t[SA[i] - 1] == "L":
                symbol = T[SA[i] - 1]
                offset = count.get(symbol, 0)
                SA[buckets[symbol][0] + offset] = SA[i] - 1
                count[symbol] = offset + 1

    count = {}
    for i in range(len(T) - 1, 0, -1):
        if SA[i] > 0:
            if t[SA[i] - 1] == "S":
                symbol = T[SA[i] - 1]
                revoffset = count[symbol] = count.get(symbol, 0) + 1
                SA[buckets[symbol][1] - revoffset] = SA[i] - 1

    return SA

#Pos -> arreglo de sufijos
#string W en A
def search(Pos, W, A):
  N = len(A)
  occurrences = []
  
  #Busca la primera ocurrencia
  L, R = 0, N
  
  while R - L > 1:
      M = (L + R) // 2
  
      if W > A[Pos[M]:]:
          L = M
      else:
          R = M
  
  start = L
  end = N
  
  #Encuentra todas las ocurrencias
  while start < end:
      if W == A[Pos[start]:Pos[start] + len(W)]:
          occurrences.append(Pos[start])
      start += 1
  
  return occurrences

#Descomentar para probar consumo de memoria
#@profile
def main():
  #Registra el tiempo de inicio
  inicio = time.time()
  
  archivo = "The Jungle Book.txt"
  texto = cuenta_palabras(archivo)
  T = [ord(c) for c in texto]
  SA = sais(T)
  #subS = input()
  #String debe de estar en minusculas
  subS = "morning"
  result = search(SA, subS, texto)
  print(len(result), "ocurrencias: ", result)
 
  
  #Registra el tiempo de finalización
  fin = time.time()
  #Calcula el tiempo transcurrido
  tiempo_transcurrido = fin - inicio

  print(f"El programa se ejecuto en: {tiempo_transcurrido} segundos")

main()