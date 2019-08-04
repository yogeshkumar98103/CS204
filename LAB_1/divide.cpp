#include<iostream>
#include<vector>
using namespace std;

typedef vector<char>::size_type SizeType;

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
vector<char> add(vector<char> num1, vector<char> num2, bool alreadyReversed = false, bool hideOverflow = false){
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
	int carry = 0, sum, remainder;

	while(i < s1){
		sum = (num1[i] - '0') + carry;
		if(i < s2){
			sum += (num2[i] - '0');
		}
		carry = sum / 10;
		remainder = sum % 10;
		
		result.push_back((remainder + '0'));
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
    num2 = add(num2, {'1'}, true, true);
		// true requests function to hide overflow

    // Subtract
    result = add(num1, num2, true, true);

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
//                   Division Function
// =======================================================

// This funcion divides two numbers stored in vectors (both should be positive)
vector<vector<char>> divide(vector<char> num1, vector<char> num2){
    vector<char> quotient;
    vector<char> remainder = {'0'};

    SizeType s1 = num1.size();
    SizeType s2 = num2.size();

    if(compare(num1, num2) == -1){
        return {{'0'}, num1};
    }

    SizeType i = 0;
    SizeType j = s2;
    bool pushZero = false;
    vector<char> part;
    remainder = getPart(num1, i, j);

    while(true){
        part = remainder;
        pushZero = false;
        
        while(j < s1 && compare(part, num2) == -1){
            part.push_back(num1[j++]);
            if(pushZero || ((remainder.size() == 1 && remainder[0] == '0') && (num1[j - 1] == '0'))) quotient.push_back('0');
            else pushZero = true;
        }
        if(j >= s1 && compare(part, num2) == -1) break;
        i = j;

        // Repeated addition till we cross `part`
        vector<char> sumTillNow = {'0'};
        int digit = 0;
        int compResult = -1;
        while(compResult == -1){
            sumTillNow = add(sumTillNow, num2);
            digit++;
            compResult = compare(sumTillNow, part);
        }
        if(compResult == 1){
            sumTillNow = subtract(sumTillNow, num2);
            digit--;
        }
        remainder = subtract(part, sumTillNow);
        quotient.push_back(digit + '0');
    }
    return {quotient, remainder};
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

        vector<vector<char>> result = divide(num1, num2);

        for(vector<char> part: result){
            for(char ch: part){
                cout << ch;
            }
            cout << endl;
        }
    }

    return 0;
}