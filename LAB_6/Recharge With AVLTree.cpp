//
// Created by Yogesh Kumar on 2019-09-04.
//

#include "AVLTree.h"
#include <iostream>
using namespace std;

const int Recharge = 1;
const int GiveMax = 2;

class Users{
    AVLTree<int, int> users_;
    int max_;
    int userIDMax_;

public:
    Users(){
        users_.setAccessBehaviour(Create_If_Not_Found);
        users_.setDefaultValue(0);
    }

    void recharge(int userID, int amount){
        users_[userID] += amount;
        if(users_[userID] > max_){
            max_ = users_[userID];
            userIDMax_ = userID;
        }
    }

    void printMax(){
        if(max_ == -1){
            cout << "No data available" << endl;
        }
        else{
            cout << userIDMax_;
        }
    }
};

int main(){
    int t, n;
    Users users;
    int commandType, userID, amount;

    cin >> t >> n;
    while(t--){
        cin >> commandType;
        switch(commandType){
            case Recharge:
                cin >> userID >> amount;
                users.recharge(userID, amount);
                break;

            case GiveMax :
                users.printMax();
                break;
        }
    }

    return 0;
}

