#include <iostream>
#include <vector>
#include <climits>

using namespace std;

struct Node{
    int userID;
    int amount;
};

class Users{
    Node* users_;
    int maxUserCount_;
    Node max_;
    vector<int>& userIds_;
    vector<int> sortedIds_;
    vector<int>& amounts_;
    vector<int>& query_;
    
  public:
    Users(vector<int>& userIds, vector<int>& amounts, vector<int>& query, int maxUserCount): userIds_(userIds), amounts_(amounts), query_(query) {
        maxUserCount_ = maxUserCount;
        users_ = new Node[maxUserCount];
        max_ = {-1,-1};

        for(int i = 0; i < maxUserCount_; i++){
            users_[i] = {INT_MAX, 0};
        }

        for(int id: userIds_){
            sortedIds_.push_back(id);
        }

        quickSort(sortedIds_, 0, sortedIds_.size() - 1);
        assign();
    }

    void evaluate(){
        int i = 0; 
        int k = 0;
        int size = query_.size();

        while(k < size){
            if(query_[k] == i){
                if(k == 0){
                    cout << "No data available" << endl;
                }
                else{
                    cout << getMax() << endl;
                }
                ++k;
            }
            else{
                recharge(userIds_[i], amounts_[i]);
                i++;
            }
        }
    }

    ~Users(){
        delete[] users_;
    }
    
  private:

    // ----------------- Quick Sort -------------------
    void quickSort(vector<int>& A, int start, int end){
        if(end > start){
            int pivotIndex = partition(A, start, end);
            quickSort(A, start,pivotIndex - 1);
            quickSort(A, pivotIndex + 1,end);
        }
    }

    int partition(vector<int>& A, int start, int end){
        int pivot = A[end];
        int i = start - 1, j = start;
        int temp;

        while(j < end){
            if(A[j] <= pivot){
                ++i;
                swap(A, i, j);
            }
            ++j;
        }

        swap(A, end, ++i);
    
        return i;
    }

    void swap(vector<int>& A, int i, int j){
        int temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }

    // ---------------- Binary Seach ------------------
    int binarySearch(int key){
        int left = 0;
        int right = maxUserCount_ - 1;
        int mid;

        while(left <= right) {
            mid = (left + right) / 2;
            if(users_[mid].userID == key){
                return mid;
            }
            else if(users_[mid].userID > key){
                right = mid - 1;
            }
            else{
                left = mid + 1;
            }
        }

        return -1;
    }

    void assign(){
        int k = 0; 
        for(int i = 0; i < sortedIds_.size(); i++){
            if(i == 0 || sortedIds_[i] != sortedIds_[i-1]){
                users_[k] = {sortedIds_[i], 0};
                k++;
            }
        }
    }
    
    void recharge(int userID, int amount){
        int index = binarySearch(userID); 
        users_[index].amount += amount;
        if(users_[index].amount > max_.amount){
            max_ = users_[index];
        }
    }

    int getMax() const{
        return max_.userID;
    }
};

const int Recharge = 1;
const int GiveMax = 2;

int main(){
    int t, n;
    cin >> t >> n;
    vector<int> userIds;
    vector<int> amounts;
    vector<int> query;

    int i = 0;
    int commandType, userID, amount;
    while(t--){
        cin >> commandType;
        switch(commandType){
            case Recharge:
                cin >> userID >> amount;
                userIds.push_back(userID);
                amounts.push_back(amount);
                ++i;
                break;

            case GiveMax : 
                query.push_back(i);
                break;
        }
    }

    Users users(userIds, amounts, query, n);
    users.evaluate();
    return 0;
}

// 15
// 10
// 2
// 1    200  500    	    
// 1    200  70       	 
// 1    52   150        
// 1    2    60       	 
// 1    5    250            
// 1    2    100        
// 1    5    150       
// 1    100  200      	 
// 2                       						
// 1	100	 300		
// 1	150	 150		
// 1	100	 350		
// 1	900	 100		
// 2