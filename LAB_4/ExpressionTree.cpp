#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <cstring>
#include <cmath>
#include <cstdio>
using namespace std;

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
    int ans;
    map<char, int> precedence;
    map<string, int> variableList;

  public:
    ExpressionTree(const string& expression, const map<string, int>& variables){
        exp = expression;
        variableList = variables;
        precedence['!'] = 4;
        precedence['^'] = 3;
        precedence['*'] = 2;
        precedence['/'] = 2;
        precedence['+'] = 1;
        precedence['-'] = 1;
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
                    string op(1, s.top());
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
                          ((s.top() == '^' && precedence[s.top()] > precedence[exp[i]]) ||
                           (s.top() != '^' && precedence[s.top()] >= precedence[exp[i]]))){
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
                if(variableList.find(current->data) == variableList.end())
                    return false;
                current->value = variableList[current->data];
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

class Compiler{
    map<string, int> variableList;

  public:

    int evaluate(string expression, bool& isValid){
        ExpressionTree tree(expression, variableList);
        tree.buildTree();
        return tree.evaluate(isValid);
    }

    string newStatement(string& statement){
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
            string variable = statement.substr(start, j + 1);

            // Ignore Whitespaces after '='
            j = i + 1;
            while(j < size && statement[j] == ' ') ++j;

            // If there is no expression just resturn value of variable
            if(j == size){
                if(variableList.find(variable) == variableList.end()) 
                    return "CANT BE EVALUATED";
                return to_string(variableList[variable]);
            }

            // Second part is Expression
            string expression = statement.substr(j, size-j);

            // Evaluate and assign
            bool isValid;
            int value = evaluate(expression, isValid);
            if(isValid){
                variableList[variable] = value;
                return "";
            }
        }
        else{
            // It is an expression
            bool isValid;
            int value = evaluate(statement, isValid);
            if(isValid)
                return to_string(value);
        }

        return "CANT BE EVALUATED";
    }
};

int main(){
    int t;
    cin >> t;
    while(t--){
        int lines;
        cin >> lines;
        cin.ignore(1,'\n');
        Compiler compiler;
        while(lines--) {
            string statement;
            getline(cin, statement);
            string result = compiler.newStatement(statement);
            if(!result.empty()){
                cout << result << endl;
            }
        }
    }

    return 0;
}