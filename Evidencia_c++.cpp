#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

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

int main() {
    std::string str = "GTCCCGATGTCATGTCAGGA$";
    std::vector<int> T(str.begin(), str.end());
    std::vector<int> SA = sais(T);
    for (int i : SA) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
}
