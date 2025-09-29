#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
    int n1 = atoi(argv[1]);
    int m1 = atoi(argv[2]);
    int n2 = atoi(argv[3]);
    int m2 = atoi(argv[4]);

    // Geração da matriz 1
    ofstream arq1("matriz1.txt");
    arq1 << n1 << " " << m1 << "\n";

    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m1; j++) {
            int val = rand() % 10 + 1;
            arq1 << val << " ";
        }
        arq1 << "\n";
    }

    arq1.close();

    // Geração da matriz 2
    ofstream arq2("matriz2.txt");
    arq2 << n2 << " " << m2 << "\n";

    for (int i = 0; i < n2; i++) {
        for (int j = 0; j < m2; j++) {
            int val = rand() % 10 + 1;
            arq2 << val << " ";
        }
        arq2 << "\n";
    }
    
    arq2.close();

    return 0;
}
