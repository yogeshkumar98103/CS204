#include <iostream>

using namespace std;


const int Recharge = 1;
const int GiveMax = 2;

class Users{
    int* users_;
    int maxUserCount_;
    int max_;
    int maxAmount_;

  public:
    
    Users(int maxUserCount){
        maxUserCount_ = maxUserCount;
        users_ = new int[maxUserCount];
        for(int i = 0; i < maxUserCount_; i++){
            users_[i] = 0;
        }

        max_ = -1;
        maxAmount_ = -1;
    }

    void recharge(int userID, int amount){
        users_[userID] += amount;
        if(users_[userID] > maxAmount_){
            max_ = userID;
            maxAmount_ = users_[userID];
        }
    }

    int giveMax() const{
        return max_;
    }
};

int main(){
    int t, n;
    cin >> t >> n;
    Users users(n);

    int commandType, userID, amount;
    while(t--){
        cin >> commandType;
        switch(commandType){
            case Recharge:
                cin >> userID >> amount;
                users.recharge(userID, amount);
                break;

            case GiveMax : 
                userID = users.giveMax();
                if(userID == -1){
                    cout << "No data available";
                }
                else{
                    cout << userID << endl;
                }
                break;
        }
    }
    return 0;
}