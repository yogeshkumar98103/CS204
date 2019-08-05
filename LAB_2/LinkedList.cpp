
#include <iostream>
#include <vector>
using namespace std;

struct Node{
    int x,y;
    Node* next;
};

// =======================================================
//              Linked List Class Specification
// =======================================================

class LinkedList{
    Node* head;
  public: 
    // Constructor
    LinkedList();

    // This function adds a node at the start of the linked list
    void AddFirst(int x, int y);

    // This function deletes the first node of linked list
    void DelFirst();

    // This function deletes the node with point containing {x,y}
    void Del(int x, int y);

    // This function gives all the points that are present atmost at d-distance from origin
    void Search(long d);

    // This function returns true if {x,y} is present in linked list
    bool Search(int x, int y); 

    // This function returns the number of nodes present in linked list
    long long Length();
    
};

// =======================================================
//                     Main Function
// =======================================================

int main(){
    LinkedList coorList;
    int t;
    cin >> t;
    int command, x, y;
    for(int i = 0; i < t; i++){
        cin >> command;

        switch(command){
            case 1: 
                cin >> x >> y;
                coorList.AddFirst(x,y);
                break;

            case 2:
                coorList.DelFirst();
                break;

            case 3:
                cin >> x >> y;
                coorList.Del(x,y);
                break;

            case 4:
                cin >> x;
                coorList.Search(x);
                break;

            case 5:
                cin >> x >> y;
                if(coorList.Search(x,y)) cout << "True" << endl;
                else cout << "False" << endl;
                break;

            case 6:
                cout << coorList.Length() << endl;
                break;

            default: 
                cout << "Exiting" << endl;
        }
    }
    
    return 0; 
}

// =======================================================
//              Linked List Class Declaration
// =======================================================
LinkedList::LinkedList(){
    head = nullptr;
}

// This function adds a node at the start of the linked list
void LinkedList::AddFirst(int x, int y){
    Node* newNode = new Node;
    newNode->x = x;
    newNode->y = y;
    newNode->next = head;
    head = newNode;
}

// This function deletes the first node of linked list
void LinkedList::DelFirst(){
    if(head == nullptr) return;
    Node* delNode = head;
    head = head->next;
    delete delNode;
}

// This function deletes the node with point containing {x,y}
void LinkedList::Del(int x, int y){
    Node* current = head;
    Node* prev = nullptr;

    while(current != nullptr){
        if(current->x == x && current->y == y){
            // Found it !!
            if(prev == nullptr){
                // Delete From Start
                DelFirst();
                return;
            }

            prev->next = current->next;
            delete current;

            return;
        }

        prev = current;
        current = current->next;
    }
}

// This function gives all the points that are present atmost at d-distance from origin
void LinkedList::Search(long d){
    Node* current = head;
    int x,y;
    while(current != nullptr){
        x = current->x;
        y = current->y;
        long distanceSquared = x * x + y * y;
        if(distanceSquared <= d * d){
            cout << "(" << x << "," << y << ") ";
        }
        
        current = current->next;
    }
    cout << endl;
}

// This function returns true if {x,y} is present in linked list
bool LinkedList::Search(int x, int y){
    Node* current = head;

    while(current != nullptr){
        if(current->x == x && current->y == y)
            return true;
        
        current = current->next;
    }

    return false;
}

// This function returns the number of nodes present in linked list
long long LinkedList::Length(){
    Node* current = head;
    long long size = 0;
    while(current != nullptr){
        current = current->next;
        size++;
    }

    return size;
}
