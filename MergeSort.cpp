#include <iostream>
#include <omp.h>
using namespace std;

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int L[n1], R[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void sequentialMergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid+1, right);
        merge(arr, left, mid, right);
    }
}

void parallelMergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelMergeSort(arr, left, mid);
            }
            #pragma omp section
            {
                parallelMergeSort(arr, mid+1, right);
            }
        }
        merge(arr, left, mid, right);
    }
}

int main() {
    int n = 8;
    int arr1[] = {38, 27, 43, 3, 9, 82, 10, 1};
    int arr2[] = {38, 27, 43, 3, 9, 82, 10, 1};

    double start = omp_get_wtime();
    sequentialMergeSort(arr1, 0, n-1);
    double seq_time = omp_get_wtime() - start;

    start = omp_get_wtime();
    parallelMergeSort(arr2, 0, n-1);
    double par_time = omp_get_wtime() - start;

    cout << "Sorted array: ";
    for (int i = 0; i < n; i++) cout << arr2[i] << " ";
    cout << endl;

    cout << "Sequential Time: " << seq_time << " sec" << endl;
    cout << "Parallel Time:   " << par_time << " sec" << endl;
    cout << "Speedup:         " << seq_time/par_time << "x" << endl;

    return 0;
}
