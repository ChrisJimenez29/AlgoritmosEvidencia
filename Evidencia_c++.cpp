#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

string readFiles(string txt){
  ifstream archivo;
  string res = "";
  string registros;
  archivo.open(txt,ios::in);

  while(!archivo.eof()){
    getline(archivo,registros);
    res += registros;
  }
  archivo.close();
  res.erase(remove_if(res.begin(), res.end(), ::isspace), res.end());
  return res;
}

map<int, pair<int, int>> getBuckets(const vector<int>& T) {
    map<int, int> count;
    map<int, pair<int, int>> buckets;
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

vector<int> sais(const vector<int>& T) {
    int n = T.size();
    vector<char> t(n, '_');

    t[n - 1] = 'S';
    for (int i = n - 2; i >= 0; i--) {
        if (T[i] < T[i + 1] || (T[i] == T[i + 1] && t[i + 1] == 'S')) {
            t[i] = 'S';
        } else {
            t[i] = 'L';
        }
    }

    auto buckets = getBuckets(T);

    vector<int> SA(n, -1);
    map<int, int> count;
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

vector<int> search(const vector<int>& Pos, const string& W, const string& A) {
  int N = A.length();
  vector<int> occurrences;

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
  auto inicio = chrono::high_resolution_clock::now();

  //Codigo
  string str = readFiles("The Jungle Book.txt");
  vector<int> T(str.begin(), str.end());
  vector<int> SA = sais(T);

  vector<int>result = search(SA, "morning", str);
  
  cout << result.size() << " ocurrencias: ";
  for (int i = 0; i < result.size(); i++) {
    cout << result[i];
    if (i != result.size() - 1) {
      cout << ", ";
    }
  }
  cout << endl;
  
  //Fin del programa en tiempo
  auto fin = chrono::high_resolution_clock::now();

  //Tiempo (nanosegundos)
  auto duracion = chrono::duration_cast<chrono::nanoseconds>(fin - inicio);

  //Tiempo (segundos)
  double duracion_segundos = duracion.count() / 1e9;

  cout << "El programa se ejecuto en " << duracion_segundos << " segundos" << endl;

    return 0;
}