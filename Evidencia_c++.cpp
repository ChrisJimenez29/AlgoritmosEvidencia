#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <chrono>

std::string readFiles(std::string archivo) {
  std::string nombreArchivo = archivo;
  std::ifstream archivoStream(nombreArchivo.c_str()); 
  if (!archivoStream.is_open()) {
    std::cerr << "No se pudo abrir el archivo: " << nombreArchivo << std::endl;
    return ""; 
  }

  std::string texto = "";
  std::string linea;

  while (std::getline(archivoStream, linea)) {
    linea.erase(std::remove_if(linea.begin(), linea.end(), ::isspace), linea.end());
    texto += linea;
  }

  archivoStream.close();
  return texto;
}


std::map<int, std::pair<int, int>> getBuckets(const std::vector<int>& T) {
    std::map<int, int> count;
    std::map<int, std::pair<int, int>> buckets;
    for (int c : T) {
        count[c]++;
    }
    int start = 0;
    for (const auto& [key, value] : count) {
        buckets[key] = {start, start + value};
        start += value;
    }
    return buckets;
}

std::vector<int> sais(const std::vector<int>& T) {
    int n = T.size();
    std::vector<char> t(n, '_');

    t[n - 1] = 'S';
    for (int i = n - 2; i >= 0; i--) {
        if (T[i] < T[i + 1] || (T[i] == T[i + 1] && t[i + 1] == 'S')) {
            t[i] = 'S';
        } else {
            t[i] = 'L';
        }
    }

    auto buckets = getBuckets(T);

    std::vector<int> SA(n, -1);
    std::map<int, int> count;
    for (int i = 0; i < n; i++) {
        if (t[i] == 'S' && (i == 0 || t[i - 1] == 'L')) {
            count[T[i]]++;
            SA[buckets[T[i]].second - count[T[i]]] = i;
        }
    }

    count.clear();
    for (int i = 0; i < n; i++) {
        if (SA[i] > 0 && t[SA[i] - 1] == 'L') {
            count[T[SA[i] - 1]]++;
            SA[buckets[T[SA[i] - 1]].first + count[T[SA[i] - 1]] - 1] = SA[i] - 1;
        }
    }

    count.clear();
    for (int i = n - 1; i >= 0; i--) {
        if (SA[i] > 0 && t[SA[i] - 1] == 'S') {
            count[T[SA[i] - 1]]++;
            SA[buckets[T[SA[i] - 1]].second - count[T[SA[i] - 1]]] = SA[i] - 1;
        }
    }

    return SA;
}

std::vector<int> search(const std::vector<int>& Pos, const std::string& W, const std::string& A) {
  int N = A.length();
  std::vector<int> occurrences;

  // Busca la primera ocurrencia
  int L = 0, R = N;

  while (R - L > 1) {
    int M = (L + R) / 2;

    if (W > A.substr(Pos[M])) {
      L = M;
    } else {
      R = M;
    }
  }

  int start = L;
  int end = N;

  // Encuentra todas las ocurrencias
  while (start < end) {
    if (W == A.substr(Pos[start], W.length())) {
      occurrences.push_back(Pos[start]);
    }
    start++;
  }

  return occurrences;
}

int main() {
  
  //Inicio de programa en tiempo
  auto inicio = std::chrono::high_resolution_clock::now();

  //Codigo
  std::string str = readFiles("Metamorphosis.txt");
  std::vector<int> T(str.begin(), str.end());
  std::vector<int> SA = sais(T);

  std::vector<int>result = search(SA, "morning", str);
  
  std::cout << result.size() << " ocurrencias: ";
  for (int i = 0; i < result.size(); i++) {
    std::cout << result[i];
    if (i != result.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << std::endl;
  
  //Fin del programa en tiempo
  auto fin = std::chrono::high_resolution_clock::now();

  //Tiempo (nanosegundos)
  auto duracion = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio);

  //Tiempo (segundos)
  double duracion_segundos = duracion.count() / 1e9;

  std::cout << "El programa se ejecuto en " << duracion_segundos << " segundos" << std::endl;

    return 0;
}