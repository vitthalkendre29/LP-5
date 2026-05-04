#include <iostream>
#include <vector>
#include <omp.h>
 
using namespace std;
 
// ---------------- Sequential Bubble Sort ----------------
void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
 
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}
 
// ---------------- Parallel Bubble Sort ----------------
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
 
    for (int phase = 0; phase < n; phase++) {
 
        // Even phase
        if (phase % 2 == 0) {
            #pragma omp parallel for
            for (int i = 1; i < n; i += 2) {
                if (arr[i - 1] > arr[i]) {
                    swap(arr[i - 1], arr[i]);
                }
            }
        }
 
        // Odd phase
        else {
            #pragma omp parallel for
            for (int i = 1; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                }
            }
        }
    }
}
 
// ---------------- Merge Function ----------------
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp;
 
    int i = left;
    int j = mid + 1;
 
    while (i <= mid && j <= right) {
        if (arr[i] < arr[j]) {
            temp.push_back(arr[i++]);
        } else {
            temp.push_back(arr[j++]);
        }
    }
 
    while (i <= mid) {
        temp.push_back(arr[i++]);
    }
 
    while (j <= right) {
        temp.push_back(arr[j++]);
    }
 
    for (int k = 0; k < temp.size(); k++) {
        arr[left + k] = temp[k];
    }
}
 
// ---------------- Sequential Merge Sort ----------------
void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left >= right)
        return;
 
    int mid = (left + right) / 2;
 
    sequentialMergeSort(arr, left, mid);
    sequentialMergeSort(arr, mid + 1, right);
 
    merge(arr, left, mid, right);
}
 
// ---------------- Parallel Merge Sort ----------------
void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left >= right)
        return;
 
    int mid = (left + right) / 2;
 
    #pragma omp parallel sections
    {
        #pragma omp section
        parallelMergeSort(arr, left, mid);
 
        #pragma omp section
        parallelMergeSort(arr, mid + 1, right);
    }
 
    merge(arr, left, mid, right);
}
 
// ---------------- Print Array ----------------
void printArray(vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}
 
// ---------------- Main Function ----------------
int main() {
    vector<int> arr = {8, 5, 3, 7, 2, 6, 1, 4};
 
    vector<int> seqBubble = arr;
    vector<int> parBubble = arr;
    vector<int> seqMerge = arr;
    vector<int> parMerge = arr;
 
    double start, end;
 
    // Sequential Bubble Sort
    start = omp_get_wtime();
    sequentialBubbleSort(seqBubble);
    end = omp_get_wtime();
    cout << "Sequential Bubble Sort: ";
    printArray(seqBubble);
    cout << "Time: " << end - start << " seconds\n\n";
 
    // Parallel Bubble Sort
    start = omp_get_wtime();
    parallelBubbleSort(parBubble);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort: ";
    printArray(parBubble);
    cout << "Time: " << end - start << " seconds\n\n";
 
    // Sequential Merge Sort
    start = omp_get_wtime();
    sequentialMergeSort(seqMerge, 0, seqMerge.size() - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort: ";
    printArray(seqMerge);
    cout << "Time: " << end - start << " seconds\n\n";
 
    // Parallel Merge Sort
    start = omp_get_wtime();
    parallelMergeSort(parMerge, 0, parMerge.size() - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort: ";
    printArray(parMerge);
    cout << "Time: " << end - start << " seconds\n";
 
    return 0;
}
