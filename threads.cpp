// Implementação com threads (p threads)
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
// #include <mutex>
// #include <utility>
#include <string>
#include <thread>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 4) {
    cerr << "Uso: " << argv[0] << " <matriz1.txt> <matriz2.txt> <P>\n";
    return 1;
  }

  string file1 = argv[1];
  string file2 = argv[2];
  int p = stoi(argv[3]);

  ifstream fin1(file1);
  int n1, m1;
  fin1 >> n1 >> m1;
  vector<vector<int>> matriz1(n1, vector<int>(m1));
  for (int i = 0; i < n1; ++i)
    for (int j = 0; j < m1; ++j)
      fin1 >> matriz1[i][j];
  fin1.close();

  ifstream fin2(file2);
  int n2, m2;
  fin2 >> n2 >> m2;
  vector<vector<int>> matriz2(n2, vector<int>(m2));
  for (int i = 0; i < n2; ++i)
    for (int j = 0; j < m2; ++j)
      fin2 >> matriz2[i][j];
  fin2.close();

  if (m1 != n2) {
    cerr << "Erro: Matrizes incompatíveis para multiplicação.\n";
    return 1;
  }

  int total_elements = n1 * m2;
  cout << total_elements << " elementos na matriz resultante.\n";
  if (p <= 0) {
    cerr << "Erro: P deve ser maior que zero.\n";
    return 1;
  }
  if (p > total_elements) {
    p = total_elements;
  }

  int num_threads = (total_elements + p - 1) / p;
  cout << num_threads << " threads disponíveis.\n";

  // Vetor para armazenar os resultados (opcional, pode ser removido)
  // vector<vector<int>> resultado(n1, vector<int>(m2, 0));

  auto compute = [&](int start_idx, int end_idx, int thread_id) {
    auto t0 = chrono::steady_clock::now();
    ofstream fout("data/threads/resultado_partes_" + to_string(thread_id) +
                  ".txt");
    for (int idx = start_idx; idx < end_idx; ++idx) {
      int i = idx / m2;
      int j = idx % m2;
      int val = 0;
      for (int k = 0; k < m1; ++k) {
        val += matriz1[i][k] * matriz2[k][j];
      }
      // resultado[i][j] = val;
      fout << "c" << setfill('0') << setw(8) << i << setw(4) << j << " " << val
           << "\n";
    }
    auto t1 = chrono::steady_clock::now();
    long ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    fout << ms << "\n";
  };

  auto total_t0 = chrono::steady_clock::now();
  vector<thread> threads;
  for (int t = 0; t < num_threads; ++t) {
    int start = t * (total_elements / num_threads) +
                min(t, total_elements % num_threads);
    int end = start + (total_elements / num_threads) +
              (t < (total_elements % num_threads) ? 1 : 0);
    threads.emplace_back(compute, start, end, t);
  }
  for (auto &th : threads)
    th.join();

  auto total_t1 = chrono::steady_clock::now();
  long total_ms =
      chrono::duration_cast<chrono::milliseconds>(total_t1 - total_t0).count();
  cout << "Tempo total(ms): " << total_ms << endl;
  return 0;
}
