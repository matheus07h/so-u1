#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
using namespace std;

// Supondo que as matrizes e variáveis já foram lidas antes deste trecho
// Exemplo de declaração (ajuste conforme necessário):
// int n1, m1, n2, m2, p;
// vector<vector<int>> matriz1, matriz2;

int main(int argc, char *argv[]) {

  if (argc != 4) {
    cerr << "Uso: " << argv[0] << " <matriz1.txt> <matriz2.txt> <P>\n";
    return 1;
  }

  ifstream fin1(argv[1]);
  int n1, m1;
  fin1 >> n1 >> m1;
  vector<vector<int>> matriz1(n1, vector<int>(m1));
  for (int i = 0; i < n1; ++i)
    for (int j = 0; j < m1; ++j)
      fin1 >> matriz1[i][j];
  fin1.close();

  // Leitura das dimensões e valores da matriz 2
  ifstream fin2(argv[2]);
  int n2, m2;
  fin2 >> n2 >> m2;
  vector<vector<int>> matriz2(n2, vector<int>(m2));
  for (int i = 0; i < n2; ++i)
    for (int j = 0; j < m2; ++j)
      fin2 >> matriz2[i][j];
  fin2.close();

  int p = stoi(argv[3]);

  auto begin = chrono::steady_clock::now();

  int total_elements = n1 * m2;
  int num_parts = (total_elements + p - 1) / p;
  cout << num_parts << " partes\n";

  for (int part = 0; part < num_parts; ++part) {
    int start = part * p;
    int end = min(start + p, total_elements);
    pid_t pid = fork();
    if (pid == 0) { // child
      auto t0 = chrono::steady_clock::now();
      ofstream fout("data/processos/resultado_parte_" + to_string(part) +
                    ".txt");
      for (int idx = start; idx < end; ++idx) {
        int i = idx / m2;
        int j = idx % m2;
        int val = 0;
        for (int k = 0; k < m1; ++k) {
          val += matriz1[i][k] * matriz2[k][j];
        }
        fout << "c" << setfill('0') << setw(8) << i << setw(2) << j << " "
             << val << "\n";
      }
      auto t1 = chrono::steady_clock::now();
      long tempo = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
      fout << tempo << "\n";
      fout.close();
      exit(0);
    }
  }
  // Espera todos os filhos terminarem
  for (int part = 0; part < num_parts; ++part) {
    wait(nullptr);
  }
  auto end = chrono::steady_clock::now();
  long tempoExecucao =
      chrono::duration_cast<chrono::milliseconds>(end - begin).count();
  cout << "Tempo total(ms): " << tempoExecucao << endl;
  return 0;
}
