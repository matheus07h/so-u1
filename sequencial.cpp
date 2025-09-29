#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

int main(int argc, char *argv[]) {
    ifstream arq1(argv[1]);
    ifstream arq2(argv[2]);

    int n1, m1, n2, m2;
    arq1 >> n1 >> m1;
    arq2 >> n2 >> m2;
   
    vector<vector<int>> matriz1(n1, vector<int>(m1));
    vector<vector<int>> matriz2(n2, vector<int>(m2));
    vector<vector<int>> resultado(n1, vector<int>(m2, 0));

    if (m1 != n2) {
        cerr << "Multiplicação impossível: m1 != n2\n";
        return 1;
    }

    // Lendo a matriz1
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m1; j++) {
            arq1 >> matriz1[i][j];
        }
    }

    arq1.close();

    // Lendo a matrix2
    for (int i = 0; i < n2; i++) {
        for (int j = 0; j < m2; j++) {
            arq2 >> matriz2[i][j];
        }
    }

    arq2.close();

    // Multiplicação
    auto begin = chrono::steady_clock::now();
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            for (int k = 0; k < m1; k++) {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }
    auto end = chrono::steady_clock::now();

    long tempoExecucao = chrono::duration_cast<chrono::milliseconds>(end - begin).count();

    ofstream arqRes("resultado.txt");
    arqRes << n1 << " " << m2 << "\n";
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            arqRes << "c" << (i+1) << (j+1) << " " << resultado[i][j] << "\n";
        }
    }
    arqRes << tempoExecucao << "\n";
    arqRes.close();

    return 0;
}
