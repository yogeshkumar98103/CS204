#include <iostream>
#include <stack>
#include <vector>
#include <cstring>
#include <cmath>
#include <cstdio>
using namespace std;

template <typename KeyType, typename ValueType>
class map{
    typedef int (*Comp_Func_Type)(const KeyType &a, const KeyType& b);

    vector<pair<KeyType, ValueType>> list;
    pair<KeyType, int> cache;

public:
    Comp_Func_Type compFn = [](const KeyType &a, const KeyType& b){
        if(a < b) return -1;
        else if(a == b) return 0;
        return 1;
    };

    map(){
        cache = {"0", -1};
    };
    map(Comp_Func_Type compFn){
        this->compFn = compFn;
        cache = {"0", -1};
    }

    void add(KeyType key, ValueType value){
        list.push_back({key, value});
    }

    void sort(){
        quickSort(0, list.size() - 1);
        cache = {"0", -1};
    }

    bool has(KeyType key){
        int index = binarySearch(key);
        if(index == -1) return false;
        cache = {key, index};
        return true;
    }

    ValueType &operator[] (KeyType key){
        if(cache.second != -1 && compFn(cache.first,key) == 0) return list[cache.second].second;
        int index = binarySearch(key);
        if(index == -1) throw out_of_range("Invalid Key");
        cache = {key, index};
        return list[index].second;
    }

    void print(){
        for(const pair<KeyType, ValueType>& ele: list){
            cout << ele.first << " : " << ele.second << endl;
        }
    }

private:

    // ----------------- Quick Sort -------------------
    void quickSort(int start, int end){
        if(end > start){
            int pivotIndex = partition(start,end);
            quickSort(start,pivotIndex - 1);
            quickSort(pivotIndex + 1,end);
        }
    }

    int partition(int start, int end){
        KeyType pivot = list[end].first;
        int i = start - 1, j = start;
        pair<KeyType, ValueType> temp;

        while(j < end){
            if(compFn(list[j].first, pivot) != 1){
                ++i;
                temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
            ++j;
        }

        temp = list[++i];
        list[i] = list[end];
        list[end] = temp;

        return i;
    }

    // ---------------- Binary Seach ------------------
    int binarySearch(KeyType key){
        int left = 0;
        int right = list.size() - 1;
        int mid;

        while(left<=right) {
            mid = (left + right) / 2;
            switch (compFn(list[mid].first, key)) {
                case 0 :
                    return mid;
                    break;
                case 1 :
                    right = mid - 1;
                    break;
                case -1 :
                    left = mid + 1;
                    break;
            }
        }

        return -1;
    }
};

typedef map<string, pair<int, int>> VariableListType;


enum TokenType{
    Variable,
    Number,
    Operator
};

struct Node{
    string data;
    int value;
    TokenType type;
    Node* leftChild;
    Node* rightChild;
};

class ExpressionTree{
    string exp;
    Node* root;
    int currentStatementIndex;
    int precedence(char c){
        int val;
        switch(c){
            case '!' : val = 4;         break;
            case '^' : val = 3;         break;
            case '*' :
            case '/' : val = 2;         break;
            case '+' :
            case '-' : val = 1;         break;
        }
        return val;
    }
    VariableListType* variableList;

public:
    ExpressionTree(const int currentStatementIndex, const string& expression, VariableListType* variables){
        this->currentStatementIndex = currentStatementIndex;
        exp = expression;
        variableList = variables;
        root = nullptr;
    }

    static bool isNumber(char c){
        return (c >= 48 && c <= 57);
    }

    static bool isAlphabet(char c){
        return (
                (c >= 65 && c <= 90) ||
                (c >= 97 && c <= 122)
        );
    }

    static bool isOperator(char c){
        return(
                c == '+' ||
                c == '-' ||
                c == '*' ||
                c == '/' ||
                c == '^' ||
                c == '!'
        );
    }

    string parse(TokenType type, int& index){
        string num;
        int i = index;
        int size = exp.size();

        switch(type){
            case Number:
                while(i < size && isNumber(exp[i])) ++i;
                break;

            case Variable:
                while(i < size && isAlphabet(exp[i])) ++i;
                break;

            default: break;
        }

        num = exp.substr(index, i - index);
        index = i - 1;
        return num;
    }

    vector<string> infixToPostFix(){
        vector<string> ans;
        stack<char> s;
        int i = 0;
        int size = exp.size();

        // Fix Unary Minus
        char prev = ' ';
        while(i < size){
            if(exp[i] == '-' && (i == 0 || prev == '(' || isOperator(prev))){
                exp[i] = '!';
            }
            if(exp[i] != ' ') prev = exp[i];
            ++i;
        }

        i = 0;

        while(i < size){
            if(exp[i] == '('){
                s.push('(');
            }
            else if(exp[i] == ')'){
                while(s.top() != '('){
                    char ch = s.top() == '!' ? '-' : s.top();
                    string op(1,ch);
                    ans.push_back(op);
                    s.pop();
                }

                s.pop();
            }
            else if(isNumber(exp[i])){
                // Parse This number
                string num = parse(Number, i);
                ans.push_back(num);
            }
            else if(isAlphabet(exp[i])){
                // Parse Variable
                string variable = parse(Variable, i);
                ans.push_back(variable);
            }
            else if(isOperator(exp[i])){
                if(exp[i] == '!'){
                    ans.push_back("0");
                }
                else{
                    while(!s.empty() && s.top() != '(' &&
                          ((s.top() == '^' && precedence(s.top()) > precedence(exp[i])) ||
                           (s.top() != '^' && precedence(s.top()) >= precedence(exp[i])))){
                        char ch = s.top() == '!' ? '-' : s.top();
                        string op(1,ch);
                        ans.push_back(op);
                        s.pop();
                    }
                }
                s.push(exp[i]);
            }

            ++i;
        }

        while(!s.empty()){
            char ch = s.top() == '!' ? '-' : s.top();
            string op(1,ch);
            ans.push_back(op);
            s.pop();
        }
        return ans;
    }

    bool evaluateTree(Node *current){
        int a,b;
        char op;

        switch(current->type){
            case Number:
                current->value = stoi(current->data);
                break;

            case Variable:
                if(!(variableList->has(current->data) && wasDeclared(current->data)))
                    return false;
                current->value = ((*variableList)[current->data]).first;
                break;

            case Operator:
                if(evaluateTree(current->leftChild) && evaluateTree(current->rightChild)){
                    a = current->rightChild->value;
                    b = current->leftChild->value;

                    op = current->data[0];
                    switch(op){
                        case '+' : current->value = a + b;          break;
                        case '-' : current->value = a - b;          break;
                        case '*' : current->value = a * b;          break;
                        case '/' : current->value = a / b;          break;
                        case '^' : current->value = pow(a,b);       break;
                    }
                    break;
                }
                else return false;
        }
        return true;
    }

    int evaluate(bool& isValid){
        isValid = evaluateTree(root);
        return root->value;
    }

    bool wasDeclared(string variable){
        int statementDeclared = (*variableList)[variable].second;
        return (statementDeclared <= currentStatementIndex);
    }

    void buildTree(){
        stack<Node*> s;
        vector<string> postfixExp = infixToPostFix();

        for(string ele: postfixExp){
            Node* current = new Node;
            current->data = ele;
            current->leftChild = nullptr;
            current->rightChild = nullptr;

            if(isNumber(ele[0])){
                current->type = Number;
            }
            else if(isAlphabet(ele[0])){
                current->type = Variable;
            }
            else if(isOperator(ele[0])){
                current->type = Operator;
            }

            if(current->type == Operator){
                current->leftChild = s.top();
                s.pop();
                current->rightChild = s.top();
                s.pop();
            }

            s.push(current);
        }

        root = s.top();
    }
};

enum StatementType{
    Expression,
    Assignment
};

struct Statement{
    string variable;
    string expression;
    StatementType type;
};

class Compiler{
    VariableListType variableList;


    int lines;
    static string defaultMessage;
    int currentStatementIndex = 0;

public:
    vector<string> results;
    vector<Statement> statements;

    Compiler(int numberOfStatements){
        lines = numberOfStatements;
        statements.reserve(lines);
        variableList.compFn = [](const string& str1, const string& str2)->int{
            int res = str1.compare(str2);
            if(res < 0) return -1;
            if(res > 0) return 1;
            return 0;
        };
    }

    void addStatement(string statement){
        Statement newStatement;
        int i = 0;
        int size = statement.size();

        // Ignore leading Whitespaces
        while(i < size && statement[i] == ' ')++i;
        int start = i;

        // Try to find equality operator
        while(i < size){
            if(statement[i] == '=') break;
            ++i;
        }

        // It is an assignment statement
        if(i < size){
            // Ignore Whitespaces before '='
            int j = i - 1;
            while(j >= 0 && statement[j] == ' ') --j;

            // First part is variable
            newStatement.variable = statement.substr(start, j + 1);
            if(!variableList.has(newStatement.variable))
                variableList.add(newStatement.variable, {0, currentStatementIndex});

            // Ignore Whitespaces after '='
            j = i + 1;
            while(j < size && statement[j] == ' ') ++j;

            // Second part is Expression
            newStatement.expression = statement.substr(j, size-j);
            newStatement.type = Assignment;

        }
        else{
            // It is an expression
            newStatement.expression = statement;
            newStatement.type = Expression;
        }
        statements.push_back(newStatement);
        ++currentStatementIndex;
    }

    void compile(){
        currentStatementIndex = 0;
        variableList.sort();
        for(Statement statement: statements){
            compileStatement(statement);
        }
    }

private:
    void compileStatement(Statement statement){
        bool isValid = false;
        int value;

        switch(statement.type){
            case Expression :
                value = evaluate(statement.expression, isValid);
                if(isValid) results.push_back(to_string(value));

                break;

            case Assignment :
                value = evaluate(statement.expression, isValid);
                if(isValid)
                    variableList[statement.variable].first = value;

                break;
        }

        if(!isValid){
            results.push_back(defaultMessage);
        }
        ++currentStatementIndex;
    }

    int evaluate(string expression, bool& isValid){
        ExpressionTree tree(currentStatementIndex, expression, &variableList);
        tree.buildTree();
        return tree.evaluate(isValid);
    }
};

string Compiler::defaultMessage = "CANT BE EVALUATED";

int main(){
    int t;
    cin >> t;
    while(t--){
        int lines;
        cin >> lines;
        cin.ignore(1,'\n');
        Compiler compiler(lines);
        while(lines--) {
            string statement;
            getline(cin, statement);
            compiler.addStatement(statement);
        }

        // Output
        compiler.compile();
        for(string result: compiler.results){
            cout << result << endl;
        }
    }

    return 0;
}