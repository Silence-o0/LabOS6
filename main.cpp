#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <syncstream>

using namespace std;

const int n = 6;
const int m = 7;
const int k = 10;

void multiply_and_print_result(vector<vector<int>>& A, vector<vector<int>>& B, vector<vector<int>>& C, int row_index, int col_index) {
    int result = 0;
    for (int i = 0; i < m; ++i) {
        result += A[row_index][i] * B[i][col_index];
    }
    osyncstream (cout) << "[" << row_index << ", " << col_index << "] = " << result << endl;
    C[row_index][col_index] = result;
}

void fill_matrix_random(vector<vector<int>>& matrix, int rows, int cols) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, 60);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = distr(gen);
        }
    }
}

int main() {
    vector<vector<int>> A(n, vector<int>(m, 0));
    vector<vector<int>> B(m, vector<int>(k, 0));
    vector<vector<int>> C(n, vector<int>(k, 0));

    fill_matrix_random(A, n, m);
    fill_matrix_random(B, m, k);

    cout << "A:" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    cout << "B:" << endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < k; ++j) {
            cout << B[i][j] << " ";
        }
        cout << endl;
    }

    vector<thread> threads;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            threads.emplace_back(multiply_and_print_result, ref(A), ref(B), ref(C), i, j);
        }
    }

    for (auto& thread : threads) {
        thread.join();
    }

    cout << "C:" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            cout << C[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
