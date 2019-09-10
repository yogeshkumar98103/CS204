#include<iostream>
#include<vector>
#include<cmath>
using namespace std;

typedef vector<char>::size_type SizeType;
int longSize = (int)(log10(pow(2, sizeof(long long) * 8)));

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
//                   Remainder/ Square Root
// =======================================================

// This funcion return remainder num1 % num2
vector<char> remainder(vector<char> num1, vector<char> num2){
    vector<char> quotient;

    SizeType s1 = num1.size();
    SizeType s2 = num2.size();

    if(compare(num1, num2) == -1){
        return num1;
    }

    SizeType i = 0;
    SizeType j = s2;
    
    vector<char> part = getPart(num1, 0, j);

    while(j <= s1){
        // Repeated addition till we cross `part`
        vector<char> sumTillNow(num2.begin(), num2.end());
        int digit = 1;
        while(compare(sumTillNow, part) == -1){
            sumTillNow = add(sumTillNow, num2);
            digit++;
        }
        if(compare(sumTillNow, part) == 1){
            sumTillNow = subtract(sumTillNow, num2);
            digit--;
        }

        part = subtract(part, sumTillNow);
        quotient.push_back(digit + '0');
        part.push_back(num1[j++]);
    }
    return part;
}

// This function approximates the square root of given vector
vector<char> approximateSquareRoot(vector<char> num){
    vector<char> approxSqrt;

    SizeType size = num.size();
    if(size >= longSize){
        SizeType exponent;
        int terms; 

        if(size % 2 == 0){
            terms = (num[0] - '0') * 10 + (num[1] - '0') + 1;
            exponent = (size/2) - 1;
        }
        else{
            terms = (num[0] - '0') * 100 + (num[1] - '0')*10 + (num[1] - '0') + 1;
            exponent = (size - 3)/2;
        }

        // Eg:- sqrt(21531312)
          // => sqrt(22 * 10^6)
          // => sqrt(22) * 10^3
          // => 5 * 10^3
          // => 5000

        // Mantissa
        int mantissa = (int)sqrt(terms) + 1;
        char digit1 = (mantissa / 10) + '0';
        char digit2 = (mantissa % 10) + '0';
        if(digit1 != '0') approxSqrt.push_back(digit1);
        approxSqrt.push_back(digit2);

        // Exponent
        for(SizeType i = 0; i < exponent; i++){
            approxSqrt.push_back('0');
        }
    }
    else{
        // For small numbers :- 

        // parse to long and find sqrt
        long long numLong = 0L;
        for(SizeType i = 0; i < size; i++){
            numLong *= 10;
            numLong += (num[i] - '0');
        }

        // Find square root
        long long sqrtLong = (long long)sqrt(numLong);

        // convert back to vector
        while(sqrtLong > 0){
            approxSqrt.push_back(sqrtLong % 10 + '0');
            sqrtLong /= 10;
        }

        reverse(approxSqrt);
    }
    
    return approxSqrt;
}

// =======================================================
//                  Check Prime Function
// =======================================================

// This function checks whether a number is prime or not
bool checkPrime(vector<char> num){
    vector<char> sqrtNum = approximateSquareRoot(num);
    vector<char> i = {'2'};

    while(compare(i, sqrtNum) != 1){
        vector<char> rem = remainder(num, i);
        if(rem[0] == '0'){
            return false;
        }
        i = add(i,{'1'});
    }
    return true;
}

// =======================================================
//                      Main Function
// =======================================================

int main(){
    int t;
    cin >> t;

    for(int i = 0; i < t; i++){
        string n;
        cin >> n;

        vector<char> num = convertToVector(n);
        trim(num);

        bool isPrime = checkPrime(num);
        if(isPrime){
            cout << "Prime" << endl;
        }
        else{
            cout << "Not a Prime" << endl;
        }
    }

    return 0;
}