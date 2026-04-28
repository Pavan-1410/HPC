#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <omp.h>
using namespace std;

void sequential_bubble_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

void parallel_bubble_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        int start = i % 2;
        #pragma omp parallel for
        for (int j = start; j < n - 1; j += 2)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
    }
}

void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);
    int i = 0, j = 0, k = l;
    while (i < (int)left.size() && j < (int)right.size())
        arr[k++] = (left[i] < right[j]) ? left[i++] : right[j++];
    while (i < (int)left.size()) arr[k++] = left[i++];
    while (j < (int)right.size()) arr[k++] = right[j++];
}

void sequential_merge_sort(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int m = (l + r) / 2;
    sequential_merge_sort(arr, l, m);
    sequential_merge_sort(arr, m + 1, r);
    merge(arr, l, m, r);
}

void parallel_merge_sort(vector<int>& arr, int l, int r, int depth = 0) {
    if (l >= r) return;
    int m = (l + r) / 2;
    if (depth < 3) {
        #pragma omp parallel sections
        {
            #pragma omp section
            parallel_merge_sort(arr, l, m, depth + 1);
            #pragma omp section
            parallel_merge_sort(arr, m + 1, r, depth + 1);
        }
    } else {
        sequential_merge_sort(arr, l, m);
        sequential_merge_sort(arr, m + 1, r);
    }
    merge(arr, l, m, r);
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    srand(time(0));
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 10000;

    vector<int> a1 = arr, a2 = arr, a3 = arr, a4 = arr;

    double t1, t2;

    t1 = omp_get_wtime();
    sequential_bubble_sort(a1);
    t2 = omp_get_wtime();
    cout << "\nSequential Bubble Sort Time: " << (t2 - t1) << " sec"; 

    t1 = omp_get_wtime();
    parallel_bubble_sort(a2);
    t2 = omp_get_wtime();
    cout << "\nParallel Bubble Sort Time:   " << (t2 - t1) << " sec";

    t1 = omp_get_wtime();
    sequential_merge_sort(a3, 0, n - 1);
    t2 = omp_get_wtime();
    cout << "\nSequential Merge Sort Time:  " << (t2 - t1) << " sec";

    t1 = omp_get_wtime();
    parallel_merge_sort(a4, 0, n - 1);
    t2 = omp_get_wtime();
    cout << "\nParallel Merge Sort Time:    " << (t2 - t1) << " sec\n";

    return 0;
}