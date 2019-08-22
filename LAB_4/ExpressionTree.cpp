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
        while(i < size){
            if(exp[i] == '-' && (i == 0 || exp[i-1] == '(' || isOperator(exp[i-1]))){
                exp[i] = '!';
            }
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

    void evaluateTree(Node *current){
        int a,b;
        char op;

        switch(current->type){
            case Number:
                current->value = stoi(current->data);
                break;

            case Variable:
                current->value = variableList[current->data];
                break;

            case Operator:
                evaluateTree(current->leftChild);
                evaluateTree(current->rightChild);

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
    }

    int evaluate(){
        evaluateTree(root);
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

    int evaluate(string expression){
        ExpressionTree tree(expression, variableList);
        tree.buildTree();
        return tree.evaluate();
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
            if(j == size) return to_string(variableList[variable]);

            // Second part is Expression
            string expression = statement.substr(j, size-j);

            // Evaluate and assign
            variableList[variable] = evaluate(expression);
        }
        else{
            // It is an expression
            return to_string(evaluate(statement));
        }

        return "";
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