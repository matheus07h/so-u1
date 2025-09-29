// Implementação com threads (p threads)
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 4) {
    cerr << "Uso: " << argv[0] << " <matriz1.txt> <matriz2.txt> <P>\n";
    return 1;
  }

  // Leitura dos argumentos
  string file1 = argv[1];
  string file2 = argv[2];
  int p = stoi(argv[3]);

  // Leitura da matriz 1
  ifstream fin1(file1);
  int n1, m1;
  fin1 >> n1 >> m1;
  vector<vector<int>> matriz1(n1, vector<int>(m1));
  for (int i = 0; i < n1; ++i)
    for (int j = 0; j < m1; ++j)
      fin1 >> matriz1[i][j];
  fin1.close();

  // Leitura da matriz 2
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

  vector<vector<int>> resultado(n1, vector<int>(m2, 0));
  mutex file_mutex;
  int total_elements = n1 * m2;
  int num_parts = (total_elements + p - 1) / p;

  auto compute_and_save = [&](int part_idx, int start_idx, int end_idx) {
    auto t0 = chrono::steady_clock::now();
    vector<pair<int, int>> positions;
    for (int idx = start_idx; idx < end_idx; ++idx) {
      int i = idx / m2;
      int j = idx % m2;
      resultado[i][j] = 0;
      for (int k = 0; k < m1; ++k) {
        resultado[i][j] += matriz1[i][k] * matriz2[k][j];
      }
      positions.emplace_back(i, j);
    }
    auto t1 = chrono::steady_clock::now();
    long tempo = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();

    string fname =
        "data/threads/resultado_parte_" + to_string(part_idx) + ".txt";
    lock_guard<mutex> lock(file_mutex);
    ofstream fout(fname);
    for (auto &pos : positions) {
      int i = pos.first;
      int j = pos.second;
      fout << "c" << setfill('0') << setw(8) << i << setw(2) << j << " "
           << resultado[i][j] << "\n";
    }
    fout << "Tempo(ms): " << tempo << "\n";
    fout.close();
  };

  auto total_t0 = chrono::steady_clock::now();
  vector<thread> threads;
  for (int part = 0; part < num_parts; ++part) {
    int start = part * p;
    int end = min(start + p, total_elements);
    threads.emplace_back(compute_and_save, part, start, end);
  }
  for (auto &t : threads)
    t.join();

  auto total_t1 = chrono::steady_clock::now();
  long total_ms =
      chrono::duration_cast<chrono::milliseconds>(total_t1 - total_t0).count();
  cout << "Tempo total(ms): " << total_ms << endl;
  return 0;
}
