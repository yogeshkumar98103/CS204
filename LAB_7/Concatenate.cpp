#include <iostream>
#include "Libraries/Sort.h"

using namespace std;


void computeLargestNum(string numbers[], int size){
    mySort(numbers, size, [](string& num1, string& num2)->bool{
        string str1 = num1 + num2;
        string str2 = num2 + num1;
        return (str1.compare(str2) > 0);
    });

    for(int i = 0; i < size; i++){
        cout << numbers[i];
    }
    cout << endl;
}

int main(){
    int t,n;
    cin >> t;
    while(t--){
        cin >> n;
        string numbers[n];
        for(int i = 0; i < n; i++){
            cin >> numbers[i];
        }

        computeLargestNum(numbers, n);
    }
    return 0;
}