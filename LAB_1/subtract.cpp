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

// This function trims leading zeros and sign inplace
void trim(vector<char> &num, bool numIsAlreadyReversed = false){
    SizeType size = num.size();
    if(size == 0) return;

    if(numIsAlreadyReversed){
        SizeType i = size - 1;
        if(num[i] == '-') i--;

        while(i > 0 && num[i] == '0'){
            num.pop_back();
            i--;
        }
    }
    else{
        SizeType i = 0;
        if(num[0] == '-') i++;
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
}

// This function compares two vectors (both should be positive)
int compare(vector<char> num1, vector<char> num2, bool numsAreAlreadyReversed = false){
    // Trim leading zeros
    trim(num1, numsAreAlreadyReversed);
    trim(num2, numsAreAlreadyReversed);

    // Declare Sizes
    int size1 = num1.size();
    int size2 = num2.size();

    // Compare on basis of size
    if(size1 > size2) return 1;
    if(size1 < size2) return -1;

    // Both vectors are of same size
    int i;
	if(numsAreAlreadyReversed){
		i = size1 - 1;
		while(i >= 0 && num1[i] == num2[i]) i--;
		if(i == 0) return 0;
	}
	else{
		i = 0;
		while(i < size1 && num1[i] == num2[i]) i++;
		if(i == size1) return 0;
	}

	if(num1[i] > num2[i]) return 1;
	return -1;
}

// =======================================================
//                  Subtract Functions
// =======================================================

// This function adds two vectors (both of them positive)
	// Inputs should be already reversed
	// It hides overflow
vector<char> add(vector<char> num1, vector<char> num2){
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

    return result;
}

// This function subtracts two vectors (both of them positive)
vector<char> subtract(vector<char> num1, vector<char> num2){
	vector<char> result;
    bool resultIsNegative = false;
	
	// Reverse both strings
	reverse(num1);
	reverse(num2);
	
	// Set num1 larger
	if(compare(num1, num2, true) == -1){
        // Second number is larger
	    vector<char> temp = num1;
		num1 = num2;
		num2 = temp;
		
        resultIsNegative = true;
	}
	
	SizeType s1 = num1.size();
	SizeType s2 = num2.size();

    // Calculate 2's complement of smaller number
    for(SizeType i = 0; i < s2; i++){
        num2[i] = ('9' - num2[i]) + '0';
    }
    for(SizeType i = s2; i < s1; i++){
        num2.push_back('9');
    }
    num2 = add(num2, {'1'});
		// true requests function to hide overflow

    // Subtract
    result = add(num1, num2);

    // Delete leading zeros;
    trim(result, true);
		// true denotes that result is already reversed

    // Add sign and reverse
    if(resultIsNegative && result[0] != '0'){
        result.push_back('-');
    }
    reverse(result);

    return result;
}

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

        vector<char> result = subtract(num1, num2);

        for(char ch: result){
            cout << ch;
        }
        cout << endl;
    }

    return 0;
}