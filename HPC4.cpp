#include <iostream>
#include <cuda_runtime.h>
 
using namespace std;
 
#define N 3
#define SIZE 10
 
// ---------------- Vector Addition Kernel ----------------
__global__ void vectorAdd(int *A, int *B, int *C, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
 
    if (i < n) {
        C[i] = A[i] + B[i];
    }
}
 
// ---------------- Matrix Multiplication Kernel ----------------
__global__ void matrixMul(int A[N][N], int B[N][N], int C[N][N]) {
    int row = threadIdx.y;
    int col = threadIdx.x;
 
    int sum = 0;
 
    for (int k = 0; k < N; k++) {
        sum += A[row][k] * B[k][col];
    }
 
    C[row][col] = sum;
}
 
int main() {
 
    // ---------------- VECTOR ADDITION ----------------
    int h_A[SIZE], h_B[SIZE], h_C[SIZE];
 
    for (int i = 0; i < SIZE; i++) {
        h_A[i] = i;
        h_B[i] = i * 2;
    }
 
    int *d_A, *d_B, *d_C;
 
    cudaMalloc((void**)&d_A, SIZE * sizeof(int));
    cudaMalloc((void**)&d_B, SIZE * sizeof(int));
    cudaMalloc((void**)&d_C, SIZE * sizeof(int));
 
    cudaMemcpy(d_A, h_A, SIZE * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, SIZE * sizeof(int), cudaMemcpyHostToDevice);
 
    vectorAdd<<<1, SIZE>>>(d_A, d_B, d_C, SIZE);
 
    cudaMemcpy(h_C, d_C, SIZE * sizeof(int), cudaMemcpyDeviceToHost);
 
    cout << "Vector Addition Result:\n";
    for (int i = 0; i < SIZE; i++) {
        cout << h_C[i] << " ";
    }
    cout << "\n\n";
 
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
 
 
    // ---------------- MATRIX MULTIPLICATION ----------------
    int h_M1[N][N] = {
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };
 
    int h_M2[N][N] = {
        {1,0,0},
        {0,1,0},
        {0,0,1}
    };
 
    int h_Result[N][N];
 
    int (*d_M1)[N], (*d_M2)[N], (*d_Result)[N];
 
    cudaMalloc((void**)&d_M1, sizeof(h_M1));
    cudaMalloc((void**)&d_M2, sizeof(h_M2));
    cudaMalloc((void**)&d_Result, sizeof(h_Result));
 
    cudaMemcpy(d_M1, h_M1, sizeof(h_M1), cudaMemcpyHostToDevice);
    cudaMemcpy(d_M2, h_M2, sizeof(h_M2), cudaMemcpyHostToDevice);
 
    dim3 threads(N, N);
 
    matrixMul<<<1, threads>>>(d_M1, d_M2, d_Result);
 
    cudaMemcpy(h_Result, d_Result, sizeof(h_Result), cudaMemcpyDeviceToHost);
 
    cout << "Matrix Multiplication Result:\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << h_Result[i][j] << " ";
        }
        cout << endl;
    }
 
    cudaFree(d_M1);
    cudaFree(d_M2);
    cudaFree(d_Result);
 
    return 0;
}
