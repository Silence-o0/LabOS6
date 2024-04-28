#include <iostream>
#include <vector>
#include <thread>
#include <random>

using namespace std;

const int n = 253;
const int m = 368;
const int k = 471;

pair<int, int> get_i_j_list(int lin) {
    int i = lin/k;
    int j = lin%k;
    return make_pair(i, j);
}

void multiply_and_print_result(vector<vector<int>>& A, vector<vector<int>>& B, vector<vector<int>>& C, int row_index, int col_index) {
    int result = 0;
    for (int i = 0; i < m; ++i) {
        result += A[row_index][i] * B[i][col_index];
    }
    C[row_index][col_index] = result;
}

void loop_calc(int num_threads, int thread_index, int cells_amount, vector<vector<int>>& A, vector<vector<int>>& B, vector<vector<int>>& C) {
    int f = cells_amount/num_threads;
    if ((thread_index+1) <= cells_amount%num_threads) {
        f++;
    }
    for (int l = 0; l < f; l++) {
        int lin = l * num_threads + thread_index;
        pair <int, int> p = get_i_j_list(lin);
        multiply_and_print_result(A, B, C, p.first, p.second);
    }
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

    const int num_threads = 10;
    vector<thread> threads;
    auto start_time = chrono::high_resolution_clock::now();
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(loop_calc, num_threads, i, n*k, ref(A), ref (B), ref(C));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    cout << " For " << num_threads << " threads: " << duration << " ms." << endl;

    return 0;
}
