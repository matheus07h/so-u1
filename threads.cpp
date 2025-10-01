#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
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

void compute(const vector<vector<int>> &matrix1,
             const vector<vector<int>> &matrix2, int cols1, int cols2,
             int start_idx, int end_idx, int thread_id) {
  auto t0 = chrono::steady_clock::now();
  ofstream fout("data/threads/result" + to_string(thread_id) + ".txt");

  for (int index = start_idx; index < end_idx; ++index) {
    int i = index / cols2;
    int j = index % cols2;
    int val = 0;
    for (int k = 0; k < cols1; ++k) {
      val += matrix1[i][k] * matrix2[k][j];
    }
    fout << "c" << setfill('0') << setw(8) << i << setw(4) << j << " " << val
         << "\n";
  }
  auto t1 = chrono::steady_clock::now();
  long ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
  fout << ms << "\n";
}

int main(int argc, char *argv[]) {
  string matrix_file1 = "matriz1.txt";
  string matrix_file2 = "matriz2.txt";
  int p = stoi(argv[1]);
  system("rm -rf data/threads/*");
  vector<vector<int>> matrix1, matrix2;
  int n1, m1, n2, m2;
  read_matrix(matrix_file1, matrix1, n1, m1);
  read_matrix(matrix_file2, matrix2, n2, m2);

  int total_elements = n1 * m2;

  int num_threads = (total_elements + p - 1) / p;
  auto t0 = chrono::steady_clock::now();

  vector<thread> threads;
  for (int t = 0; t < num_threads; ++t) {
    int start = t * p;
    int end = min(start + p, total_elements);
    threads.emplace_back(compute, cref(matrix1), cref(matrix2), m1, m2, start,
                         end, t);
  }

  for (auto &th : threads)
    th.join();

  auto t1 = chrono::steady_clock::now();
  long ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
  cout << "Tempo total: " << ms << " ms" << endl;
  return 0;
}
