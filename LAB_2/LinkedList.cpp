
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
    int AddFirst(int x, int y);

    // This function deletes the first node of linked list
    int DelFirst();

    // This function deletes the node with point containing {x,y}
    int Del(int x, int y);

    // This function gives all the points that are present atmost at d-distance from origin
    long long Search(double d);

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
    long long t;
    cin >> t;
    int command, x, y;
    vector<Node*> list;

    for(long long i = 0; i < t; i++){
        cin >> command;

        switch(command){
            case 1: 
                cin >> x >> y;
                coorList.AddFirst(x,y);
                break;

            case 2:
                if(coorList.DelFirst() == -1){
                    cout << -1 << endl;   
                }
                break;

            case 3:
                cin >> x >> y;
                if(coorList.Del(x,y) == -1){
                    cout << -1 << endl;   
                }
                break;

            case 4:
                cin >> x;
                cout << coorList.Search(x) << endl;
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
int LinkedList::AddFirst(int x, int y){
    Node* newNode = new Node;

    if(newNode == nullptr){
        // Cannot create new Node
        return -1;
    }

    newNode->x = x;
    newNode->y = y;
    newNode->next = head;
    head = newNode;

    return 0;
}

// This function deletes the first node of linked list
int LinkedList::DelFirst(){
    if(head == nullptr) return -1;
    Node* delNode = head;
    head = head->next;
    delete delNode;
    return 0;
}

// This function deletes the node with point containing {x,y}
int LinkedList::Del(int x, int y){
    Node* current = head;
    Node* prev = nullptr;

    while(current != nullptr){
        if(current->x == x && current->y == y){
            // Found it !!
            if(prev == nullptr){
                // Delete From Start
                return DelFirst();
            }

            prev->next = current->next;
            delete current;

            return 0;
        }

        prev = current;
        current = current->next;
    }

    return -1;
}

// This function gives all the points that are present atmost at d-distance from origin
long long LinkedList::Search(double d){
    Node* current = head;
    int x,y;
    long long count = 0LL;
    while(current != nullptr){
        x = current->x;
        y = current->y;
        long long distanceSquared = x * x + y * y;
       
        if(distanceSquared <= (d * d)){
            count++;
        }
        
        current = current->next;
    }
    
    if(count == 0) return -1;
    return count;
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
