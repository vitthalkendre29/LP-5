#include <iostream>
#include <omp.h>
using namespace std;

void sequentialBubbleSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

void parallelBubbleSort(int arr[], int n) {
    for (int k = 0; k < n; k++) {
        if (k % 2 == 0) {
            #pragma omp parallel for
            for (int i = 0; i < n-1; i += 2) {
                if (arr[i] > arr[i+1]) {
                    swap(arr[i], arr[i+1]);
                }
            }
        } else {
            #pragma omp parallel for
            for (int i = 1; i < n-1; i += 2) {
                if (arr[i] > arr[i+1]) {
                    swap(arr[i], arr[i+1]);
                }
            }
        }
    }
}

int main() {
    int n = 8;
    int arr1[] = {64, 34, 25, 12, 22, 11, 90, 45};
    int arr2[] = {64, 34, 25, 12, 22, 11, 90, 45};

    double start = omp_get_wtime();
    sequentialBubbleSort(arr1, n);
    double seq_time = omp_get_wtime() - start;

    start = omp_get_wtime();
    parallelBubbleSort(arr2, n);
    double par_time = omp_get_wtime() - start;

    cout << "Sorted array: ";
    for (int i = 0; i < n; i++) cout << arr2[i] << " ";
    cout << endl;

    cout << "Sequential Time: " << seq_time << " sec" << endl;
    cout << "Parallel Time:   " << par_time << " sec" << endl;
    cout << "Speedup:         " << seq_time/par_time << "x" << endl;

    return 0;
}
