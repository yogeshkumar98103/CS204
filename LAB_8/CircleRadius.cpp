#include <iostream>
using namespace std;

template<typename T>
void insertionSort(T* A, int size){
    for(int i = 1; i < size; i++){
        int j = i - 1;
        T temp = A[i];
        while( j >= 0 && A[j] > A[i]){
            A[j + 1] = A[j];
            j--;
        }
        A[j+1] = temp;
    }
}

const int K = 5;
const int R = 71;

template<typename Pointer>
void Swap(Pointer x, Pointer y){
    auto temp = *x;
    *x = *y;
    *y = temp;
}


template<typename T>
T partition(T* A, int size, T pivot){
    for(int i = 0; i < size; i++){
        if(A[i] == pivot){
            swap(A[i], A[size - 1]);
            break;
        }
    }

    T* i = A - 1;
    T* j = A;

    while(j < A + size - 1){
        if(*j < pivot){
            Swap(++i, j);
        }
        ++j;
    }

    Swap(++i, &A[size - 1]);
    return i - A;
}

template <typename T>
T kthSmallest(T *A, int size, int k){
    if(size <= 71){
        insertionSort(A, size);
        int mid = size/2;
        return A[mid];
    }    

    int groupCount = (size + K - 1)/k;
    int lastGroupSize = size - K*(groupCount - 1);
    T medians[groupCount];

    int i;
    for(i = 0; i < groupCount - 1; i++){
        insertionSort(A + i*K, K);
    }
    insertionSort(A + K*i, lastGroupSize);

    
    for(i = 0; i < groupCount - 1; i++){
        medians[i] = A[i*K + K/2];
    }
    medians[groupCount - 1] = A[K*i + lastGroupSize/2];

    T median_of_medians =  kthSmallest(medians, groupCount, groupCount/2);
    int pivotIndex = partition(A, size, median_of_medians);
    int mid = size/2;

    if (pivotIndex == mid-1) 
        return A[pivotIndex]; 
    if (pivotIndex > mid-1) 
        return kthSmallest(A, pivotIndex, k); 

    return kthSmallest(A + pivotIndex + 1, size - pivotIndex - 1, k - pivotIndex - 1);
}



int main(){
    int distances[10] = {2,4,5,1,3,7,6,9,8,0};
    int minRadius = kthSmallest(distances, 10, 5);
    cout << minRadius << endl;
    return 0;
    int t, k, x, y;
    cin >> t;
    while(t--){
        cin >> k;
        long long distances[k];
        for(int i = 0; i < k; i++){
            cin >> x >> y;
            distances[i] = x*x + y*y;
        }

        double minRadius = kthSmallest(distances, k, k/2);
    }

    return 0;
}