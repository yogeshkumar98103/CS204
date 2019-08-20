#include <iostream>
using namespace std;

enum NodeType{
    primitive,
    variable,
    unaryOperator,
    binaryOperator,
};

struct Node{
    string data;
    int value;
    NodeType type;
    Node* operandList[2];
};

class ExpressionTree{
    Node* root;
    string expression;

  public:
    
    bool isOperator(int index){
        return(
            expression[index] == '+' || 
            expression[index] == '-' || 
            expression[index] == '*' || 
            expression[index] == '/' || 
            expression[index] == '^'
        ); 
    }

    int flatten(int start, int end, int& innerStart, int& innerEnd){
        int level = start;
        while(level <= end && expression[level] == '(') level++;

        int count = level - start;
        int i = level;
        while(i <= end && count != 0){
            if(expression[i] == '(') ++count;
            else if(expression[i] == ')') --count;
            i++;
        }

        if(count != 0){
            // Invalid Expression;
            return -1;
        }

        innerStart = level;

        if(i == level){
            // Not a nested Expression
                // Got till first operator
            while(i <= end && !isOperator(i)) i++;
            innerEnd = i - 1;
        }
        else{
            innerEnd = i - level - 1;
        }

        return (level - start);
    }

    Node* buildTree(int start, int end){
        Node* current = new Node;
        int size = expression.size();
        int leftStart, leftEnd, rightStart, rightEnd;

        int level = flatten(start, end, leftStart, leftEnd);

        if(level == -1){
            // In valid Expression
        }

        if(leftEnd + level == end){
            if(expression[leftStart] == '-'){
                current->data = '-';
                current->type = unaryOperator;
            }
            else{
                current->data = expression.substr(leftStart, leftEnd - leftStart + 1);
                current->type = primitive;
                // TODO: Add support for variable
            }
        }

        else{
            level = flatten(leftEnd + level + 2, end, rightStart, rightEnd);
            if(level == -1){
                // In valid Expression
            }

            current->type = binaryOperator;
            current->data = expression[leftEnd + level + 1];
            current->operandList[0] = buildTree(leftStart, leftEnd);
            current->operandList[1] = buildTree(rightStart, rightEnd);

        }

        

        // First part is a nested expression (E1)__op__E2
            
            else{
                // Expression is invalid

            }
            
        // }
        // else{
            // First expression is simple expression

            while(true){
                if()
            }

            current->operandList[0] = buildTree(leftExpression);
            current->operandList[1] = buildTree(rightExpression);
        // }
        
        return current;
    }

    Node* evaluate(){

    }
}
