#include<iostream>
#include<vector>
using namespace std;

typedef vector<char>::size_type SizeType;
#define deb(x) cout << #x << " : " << x << endl;
#define printVector(x) for(SizeType i = 0; i < x.size(); i++) cout << x[i]; cout << endl;

// =======================================================
//                  Helper Functions
// =======================================================

// This function converts given string to vector
vector<char> convertToVector(const string & num){
    vector<char> convertedVector;
    for(SizeType i = 0; i < num.size(); i++){
        convertedVector.push_back(num[i]);
    }
    return convertedVector;
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

// This part return a slice of given vector
vector<char> getPart(vector<char> num, SizeType start, SizeType end){
    end = min(end, num.size());
    vector<char> part(end - start);
    for(SizeType i = start; i < end; i++){
        part[i - start] = num[i];
    }
    return part;
}

// This function concatenates two vectors
vector<char> concatenate(vector<char> part1, vector<char> part2){
    // Define size constants
    SizeType s1 = part1.size();
    SizeType s2 = part2.size();

    // This will contain resultant number
    vector<char> whole;
    
    // Ignore leading zeros of first number
    SizeType start = 0;
    while(start < s1 && part1[start] == '0') start++;

    // Add digits to resultant number
    for(SizeType i = start; i < s1; i++) whole.push_back(part1[i]);
    for(SizeType j = 0; j < s2; j++)     whole.push_back(part2[j]);

    return whole;
}

// =======================================================
//                     Add/Subtract
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

// This function subtracts two vectors (both of them positive)
vector<char> subtractPositive(vector<char> num1, vector<char> num2){
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
    num2 = addPositive(num2, {'1'}, true, true);
		// true requests function to hide overflow

    // Subtract
    result = addPositive(num1, num2, true, true);

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

// =======================================================
//                   Division Functions
// =======================================================

// This funcion divides two numbers stored in vectors (both should be positive)
vector<vector<char>> dividePositive(vector<char> num1, vector<char> num2){
    vector<char> quotient;
    vector<char> remainder;

    SizeType s1 = num1.size();
    SizeType s2 = num2.size();

    if(compare(num1, num2) == -1){
        return {{'0'}, num1};
    }

    vector<char> rem = {'0'};

    SizeType i = 0;
    SizeType j = s2;
    bool pushZero = false;
    vector<char> part;
    rem = getPart(num1, i, j);

    while(true){
        part = rem;
        pushZero = false;
        // cout << "PART : "; printVector(part);
        while(j < s1 && compare(part, num2) == -1){
            part.push_back(num1[j++]);
            if(pushZero || ((rem.size() == 1 && rem[0] == '0') && (num1[j - 1] == '0'))) quotient.push_back('0');
            else pushZero = true;
            // cout << "PART : "; printVector(part);
            // cout << "Quotient : "; printVector(quotient);
        }
        if(j >= s1 && compare(part, num2) == -1) break;
        i = j;

        // Repeated addition till we cross `part`
        vector<char> sumTillNow = {'0'};
        int digit = 0;
        int compResult = -1;
        while(compResult == -1){
            sumTillNow = addPositive(sumTillNow, num2);
            digit++;
            compResult = compare(sumTillNow, part);
        }
        if(compResult == 1){
            sumTillNow = subtractPositive(sumTillNow, num2);
            digit--;
        }
        rem = subtractPositive(part, sumTillNow);
        // cout << "DIGIT : " << digit << "\t";
        // cout << "REM : "; printVector(rem);
        quotient.push_back(digit + '0');
    }
    remainder = rem;
    return {quotient, remainder};
}

// This function divides two numbers stored in string (any sign)
vector<vector<char>> divide(string n1, string n2){
    // Convert these strings to vectors
    vector<char> num1 = convertToVector(n1);
    vector<char> num2 = convertToVector(n2);

    // Handle Sign
    bool resultIsNegative = (num1[0] == '-')^(num2[0] == '-');

    // Trim leading zeros and sign
    trim(num1);
    trim(num2);

    // Handle divide by zero
    if(num2[0] == '0'){
        cout << "Cannot divide by zero" << endl;
        exit(1);
    }

    // Divide these positive numbers
    vector<vector<char>> result = dividePositive(num1, num2);

    // Handle Sign
    if(resultIsNegative){
        // Add -ve sign to quotient
        if(result[0][0] != '0')
            result[0].insert(result[0].begin(), '-');

        // Modify remainder
        if(result[1][0] != '0')
            result[1] = subtractPositive(num2, result[1]);
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

    // Divide
    vector<vector<char>> result = divide(num1, num2);
    vector<char> quotient = result[0];
    vector<char> remainder = result[1];

    // Print Quotient
    cout << "Quotient : ";
    for(char ele: quotient) cout << ele;
    cout << endl;

    // Print Remainder
    cout << "Remainder : ";
    for(char ele: remainder) cout << ele;
    cout << endl;

    return 0;
}