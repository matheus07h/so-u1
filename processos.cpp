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
void read_matrix(const string &filename, vector<vector<int>> &matrix, int &rows,
                 int &cols) {
  ifstream fin(filename);
  fin >> rows >> cols;
  matrix.resize(rows, vector<int>(cols));
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      fin >> matrix[i][j];
  fin.close();
}

void compute(const vector<vector<int>> &matriz1,
             const vector<vector<int>> &matriz2, int m1, int m2, int start,
             int end, int part) {
  auto t0 = chrono::steady_clock::now();
  ofstream fout("data/processos/resultado" + to_string(part) + ".txt");

  for (int index = start; index < end; ++index) {
    int i = index / m2;
    int j = index % m2;
    int val = 0;
    for (int k = 0; k < m1; ++k) {
      val += matriz1[i][k] * matriz2[k][j];
    }
    fout << "c" << setfill('0') << setw(8) << i << setw(2) << j << " " << val
         << "\n";
  }
  auto t1 = chrono::steady_clock::now();
  long tempo = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
  fout << tempo << "\n";
  fout.close();
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    cerr << "Uso: " << argv[0] << " <P>\n";
    return 1;
  }

  vector<vector<int>> matriz1, matriz2;
  int n1, m1, n2, m2;
  read_matrix("matriz1.txt", matriz1, n1, m1);
  read_matrix("matriz2.txt", matriz2, n2, m2);

  int p = stoi(argv[1]);

  system("rm -rf data/processos/*");
  int total_elements = n1 * m2;
  int num_parts = (total_elements + p - 1) / p;

  auto t0 = chrono::steady_clock::now();
  for (int part = 0; part < num_parts; ++part) {
    int start = part * p;
    int end = min(start + p, total_elements);
    pid_t pid = fork();
    if (pid == 0) {
      compute(matriz1, matriz2, m1, m2, start, end, part);
      exit(0);
    }
  }

  for (int part = 0; part < num_parts; ++part) {
    wait(nullptr);
  }

  auto t1 = chrono::steady_clock::now();
  long ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
  cout << "Execution: " << ms << " ms" << endl;
  return 0;
}
