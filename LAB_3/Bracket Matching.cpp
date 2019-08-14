#include <iostream>

using namespace std;

struct Node{
    char bracket;
    Node* next;
};

class Stack{
    Node* head;

  public:
    Stack(){
        head = nullptr;
    }

    char pop(){
        if(head == nullptr) return '\0';
        char poppedEle = head->bracket;
        head = head->next;
        return poppedEle;
    }

    void push(char bracket){
        Node* newNode = new Node;
        newNode->bracket = bracket;
        newNode->next = head;
        head = newNode;
    }

    bool isEmpty(){
        return (head == nullptr);
    }
};

bool isLeftBracket(char c){
    return (c == '(' || c == '{' || c == '[' || c == '<');
}

bool areComplementary(char left, char right){
    return (
        (left == '(' && right == ')') ||
        (left == '{' && right == '}') ||
        (left == '[' && right == ']') ||
        (left == '<' && right == '>')
    );
}

int main(){
    int t;
    cin >> t;
    string str;
    while(t--){
        cin >> str;
        Stack stack;
        string ans = "YES";
        for(char c: str){
            if(isLeftBracket(c)){
                stack.push(c);
            }
            else if(c == '|'){
                char top = stack.pop();
                if(top != '|'){
                    if(top != '\0')
                        stack.push(top);
                    stack.push('|');
                }
            }
            else if(!areComplementary(stack.pop(), c)){
                ans = "NO";
                break;
            }
        }

        if(!stack.isEmpty()){
            ans = "NO";
        }
        cout << ans << endl;
    }

    return 0;
}