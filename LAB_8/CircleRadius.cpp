#include <iostream>
#include <cmath>
using namespace std;

const int K = 5;           // This is the size of each bucket
const int R = 71;          // This reprents the size of base case

/* 
 * This function performs insertion sort on given Array
 */
template<typename T>
void insertionSort(T* A, int size){
    for(int i = 1; i < size; i++){
        int j = i - 1;
        T temp = A[i];
        while( j >= 0 && A[j] > temp){
            A[j + 1] = A[j];
            j--;
        }
        A[j+1] = temp;
    }
}

/* 
 * This function swaps the value of given two pointers
 */
template<typename Pointer>
void swapPointerValues(Pointer x, Pointer y){
    auto temp = *x;
    *x = *y;
    *y = temp;
}

/*
 * This function creates a parition based on given pivot
 * and returns the index of pivot in partitioned array.
 */
template<typename T>
T partition(T* A, int size, T pivot){
    for(int i = 0; i < size; i++){
        if(A[i] == pivot){
            swap(A[i], A[size - 1]);
            break;
        }
    }

    T *i = A - 1, *j = A;

    while(j < A + size - 1){
        if(*j < pivot) swapPointerValues(++i, j);
        ++j;
    }

    swapPointerValues(++i, &A[size - 1]);
    return i - A;
}

/* 
 * This function finds the kth smallest element in given array
 */
template <typename T>
T kthSmallest(T *A, int size, int k){
    if(size <= R){
        insertionSort(A, size);
        int mid = (size - 1)/2;
        return A[mid];
    }

    int groupCount = (size + K - 1)/K;
    int lastGroupSize = size - K*(groupCount - 1);
    T medians[groupCount];

    int i;
    for(i = 0; i < groupCount - 1; i++){
        insertionSort(A + i*K, K);
    }
    insertionSort(A + K*i, lastGroupSize);


    for(i = 0; i < groupCount - 1; i++){
        medians[i] = A[i*K + (K-1)/2];
    }
    medians[groupCount - 1] = A[K*i + (lastGroupSize - 1)/2];

    T median_of_medians =  kthSmallest(medians, groupCount, (groupCount + 1)/2);
    int pivotIndex = partition(A, size, median_of_medians);

    if (pivotIndex == k-1)
        return A[pivotIndex];
    if (pivotIndex > k-1)
        return kthSmallest(A, pivotIndex, k);

    return kthSmallest(A + pivotIndex + 1, size - pivotIndex - 1, k - pivotIndex - 1);
}

/* 
 * This function finds the median of given array
 */
double findMedian(long long arr[], int size){
    long long* ptr = arr;
    return sqrt(kthSmallest(ptr, size, (size+1)/2));
}

int main(){
    int t, k, x, y;
    cin >> t;
    while(t--){
        cin >> k;
        long long distances[k];
        for(int i = 0; i < k; i++){
            cin >> x >> y;
            distances[i] = x*x + y*y;
        }
        double minRadius = findMedian(distances, k);
        cout << minRadius << endl;
    }

    return 0;
}