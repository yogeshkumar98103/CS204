#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <cstring>
#include <cmath>
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

public:

    map<char, int> precedence;

    ExpressionTree(string expression){
        exp = expression;
        precedence['^'] = 3;
        precedence['*'] = 2;
        precedence['/'] = 2;
        precedence['+'] = 1;
        precedence['-'] = 1;
    }

    bool isNumber(char c){
        return (c >= 48 && c <= 57);
    }

    bool isAlphabet(char c){
        return (
                (c >= 65 && c <= 90) ||
                (c >= 97 && c <= 122)
        );
    }

    bool isOperator(char c){
        return(
                c == '+' ||
                c == '-' ||
                c == '*' ||
                c == '/' ||
                c == '^'
        );
    }



    string parse(TokenType type, int& index){
        string num;
        int i = index;
        int size = exp.size();

        switch(type){
            case Number:
                while(i < size && isNumber(exp[i])){
                    i++;
                }
                break;

            case Variable:
                while(i < size && isAlphabet(exp[i])){
                    i++;
                }
                break;
        }



        num = exp.substr(index, i - index);
        index = i - 1;
        return num;
    }

    string parseVariable(int& index){
        string num;
        int i = index;
        int size = exp.size();

        while(i < size && isNumber(exp[i])){
            i++;
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
                while(!s.empty() && s.top() != '(' && precedence[s.top()] > precedence[exp[i]]){
                    string op(1,s.top());
                    ans.push_back(op);
                    s.pop();
                }

                s.push(exp[i]);
            }


            ++i;
        }

        while(!s.empty()){
            string op(1, s.top());
            ans.push_back(op);
            s.pop();
        }
        return ans;
    }

    void evaluate(Node *current){
        if(current->type != Operator){
            current->value = stoi(current->data);
        }
        else{
            evaluate(current->leftChild);
            evaluate(current->rightChild);

            int a = current->rightChild->value;
            int b = current->leftChild->value;


            char op = current->data[0];
            switch(op){
                case '+' : current->value = a + b;          break;
                case '-' : current->value = a - b;          break;
                case '*' : current->value = a * b;          break;
                case '/' : current->value = a / b;          break;
                case '^' : current->value = pow(a,b);       break;
            }
        }
    }

    int evaluateTree(){
        evaluate(root);
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



int main(){
    int t;
    cin >> t;
    while(t--){
        string exp;
        cin >> exp;
        ExpressionTree tree(exp);
        tree.buildTree();
        cout << tree.evaluateTree() << endl;
    }


    return 0;
}