#include <iostream>
#include <climits>
#include <omp.h>
using namespace std;

int main() {
    int n = 8;
    int arr[] = {5, 3, 8, 1, 9, 2, 7, 4};

    int min_val, max_val;
    long long sum;
    double avg;

    cout << "Array: ";
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << endl << endl;

    // ============ SEQUENTIAL ============
    min_val = INT_MAX;
    max_val = INT_MIN;
    sum = 0;

    double start = omp_get_wtime();

    for (int i = 0; i < n; i++) {
        if (arr[i] < min_val) min_val = arr[i];
        if (arr[i] > max_val) max_val = arr[i];
        sum += arr[i];
    }
    avg = (double)sum / n;

    double seq_time = omp_get_wtime() - start;

    cout << "=== SEQUENTIAL ===" << endl;
    cout << "Min     = " << min_val << endl;
    cout << "Max     = " << max_val << endl;
    cout << "Sum     = " << sum << endl;
    cout << "Average = " << avg << endl;
    cout << "Time    = " << seq_time << " sec" << endl;

    // ============ PARALLEL ============
    min_val = INT_MAX;
    max_val = INT_MIN;
    sum = 0;

    start = omp_get_wtime();

    #pragma omp parallel for reduction(min: min_val)
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_val)
            min_val = arr[i];
    }

    #pragma omp parallel for reduction(max: max_val)
    for (int i = 0; i < n; i++) {
        if (arr[i] > max_val)
            max_val = arr[i];
    }

    #pragma omp parallel for reduction(+: sum)
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    avg = (double)sum / n;

    double par_time = omp_get_wtime() - start;

    cout << "\n=== PARALLEL ===" << endl;
    cout << "Min     = " << min_val << endl;
    cout << "Max     = " << max_val << endl;
    cout << "Sum     = " << sum << endl;
    cout << "Average = " << avg << endl;
    cout << "Time    = " << par_time << " sec" << endl;

    cout << "\n=== PERFORMANCE ===" << endl;
    cout << "Speedup = " << seq_time / par_time << "x" << endl;

    return 0;
}
