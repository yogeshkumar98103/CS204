#include<iostream>
#include<vector>
using namespace std;

typedef vector<char>::size_type SizeType;

// =======================================================
//                  Helper Functions
// =======================================================

// This function reverses given vector inplace
vector<char> convertToVector(string num){
    vector<char> result;
    for(SizeType i = 0; i < num.size(); i++){
        result.push_back(num[i]);
    }
    return result;
}

// This function reverses given vector inplace
void reverse(vector<char> &num){
    int i = 0;
    int j = num.size() - 1;
    int temp;

    while(i < j){
        // Swap positions i and j
        temp = num[i];
        num[i] = num[j];
        num[j] = temp;

        i++;
        j--;
    }
}

// This function trims leading zeros
void trim(vector<char> &num){
    SizeType size = num.size();
    if(size == 0) return;

    SizeType i = 0;
    while(i < size - 1 && num[i] == '0') i++;

    // Shift by i places;
    if(i != 0){
        for(SizeType j = i; j < size; j++){
            num[j - i] = num[j];
        }
        for(SizeType j = 0; j < i; j++){
            num.pop_back();
        }
    }
}


// =======================================================
//                     Add Function
// =======================================================

// This function adds two vectors (both of them positive)
vector<char> add(vector<char> num1, vector<char> num2){

    reverse(num1);
    reverse(num2);

    vector<char> result;

    // Set num1 to be larger
    if(num1.size() < num2.size()){
        vector<char> temp = num1;
        num1 = num2;
        num2 = temp;
    }

    SizeType s1 = num1.size();
    SizeType s2 = num2.size();
    SizeType i = 0;

    int carry = 0, sum, rem;

    while(i < s1){
        sum = (num1[i] - '0') + carry;
        if(i < s2){
            sum += (num2[i] - '0');
        }
        carry = sum / 10;
        rem = sum % 10;
        result.push_back((rem + '0'));

        i++;
    }

    if(carry != 0){
        result.push_back((carry + '0'));
    }

    reverse(result);

    return result;
}

// =======================================================
//                     Main Function
// =======================================================


int main(){
    int t;
    cin >> t;

    for(int i = 0; i < t; i++){
        string n1, n2;
        cin >> n1;
        cin >> n2;

        vector<char> num1 = convertToVector(n1);
        vector<char> num2 = convertToVector(n2);

        trim(num1);
        trim(num2);

        vector<char> result = add(num1, num2);

        for(char ch: result){
            cout << ch;
        }
        cout << endl;
    }

    return 0;
}