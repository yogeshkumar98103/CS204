#include<iostream>
#include<vector>
using namespace std;

typedef vector<char>::size_type SizeType;
#define deb(x) cout << #x << " : " << x << endl;
#define print(x) for(SizeType i = 0; i < x.size(); i++) cout << x[i]; cout << endl;

void printReverse(vector<char> x){
    for(int i = x.size() - 1; i >= 0; i--){
        cout << x[i]; 
    }
    cout << endl;
}
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
//                Multiplication Functions
// =======================================================

// This function adds two vectors (both of them positive)
vector<char> addPositive(vector<char> num1, vector<char> num2, bool alreadyReversed = false, bool hideOverflow = false){
    if(!alreadyReversed){
		reverse(num1);
		reverse(num2);
	}
	
	vector<char> result;

    // Set num1 to be larger
	if(compare(num1, num2, true) == -1){
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
    
	if(carry != 0 && !hideOverflow){
	    result.push_back((carry + '0'));   
	}

	if(!alreadyReversed){
		reverse(result);
	}

    return result;
}

// This function multiplies two numbers stored in vector
vector<char> multiplyPositive(vector<char> num1, vector<char> num2){
    reverse(num1);
    reverse(num2);

    vector<char> result;

    SizeType s1 = num1.size();
    SizeType s2 = num2.size();

    int mult, rem, carry = 0;
    vector<char> tempSum;

    for(SizeType j = 0; j < s2; j++){
        int n2 = (num2[j] - '0');
        carry = 0;
        tempSum = {};

        // Give Padding
        for(SizeType k = 0; k < j; k++) tempSum.push_back('0');

        for(SizeType i = 0; i < s1; i++){
            int n1 = (num1[i] - '0');
            
            // Perform digit wise multiplication
            mult = n1 * n2 + carry;
            rem = mult % 10;
            carry = mult / 10;
            
            tempSum.push_back(rem + '0'); 
        }

        // Handle carry after last digit multiplication
        if(carry != 0){
            tempSum.push_back(carry + '0'); 
        }

        // Maintain a sum of `tempSum` till now
        result = addPositive(tempSum, result, true);
    }
    
    reverse(result);
    return result;
}

vector<char> multiply(string n1, string n2){
    // Convert these strings to vectors
    vector<char> num1 = convertToVector(n1);
    vector<char> num2 = convertToVector(n2);

    // Handle Sign
    bool resultIsNegative = (num1[0] == '-')^(num2[0] == '-');

    // Trim leading zeros and sign
    trim(num1);
    trim(num2);

    // Multiply these positive numbers
    vector<char> result = multiplyPositive(num1, num2);

    // Handle Sign
    if(resultIsNegative){
        if(result[0] != '0')
            result.insert(result.begin(), '-');
    }
    
    return result;
}

// =======================================================
//                     Main Function
// =======================================================

int main(){
    string num1, num2;

    // Input two numbers as string
    cout << "Enter number 1 : "; 
    cin >> num1;
    cout << "Enter number 2 : ";
    cin >> num2;

    // Multiply
    vector<char> result = multiply(num1, num2);

    // Print Result
    for(char ele: result){
        cout << ele;
    }
    cout << endl;

    return 0;
}