#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

typedef pair<int, int>                  Point;
typedef vector<Point>                   BlocksType;
typedef vector<bool>                    VisitedType;

static const vector<Point> directions = {{1,0},{-1,0},{0,1},{0,-1}};

int binarySearch(const vector<Point>& A, Point x) {
    int l = 0;
    int r = A.size() - 1;
    while(r >= l) {
        int mid = l + (r - l) / 2;
        if (A[mid] == x)
            return mid;

        if (A[mid] > x)
            r = mid - 1;
        else{
            l = mid + 1;
        }
    }
    return -1;
}

inline bool isValid(int x, int y, const int m, const int n, VisitedType& visited, const BlocksType& blocks, int& index){
    bool boundaryConstraint = (x >= 0 && x < m) && (y >= 0 && y < n);
    if(boundaryConstraint){
        index = binarySearch(blocks, make_pair(x,y));
        return (index != -1) && (!visited[index]);
    }
    return false;
}

void DFS(int index, const int m, const int n, VisitedType& visited, const vector<Point>& blocks, long long &result){
    visited[index] = true;
    int x = blocks[index].first;
    int y = blocks[index].second;
    int newX, newY;

    // Look in all directions
    for(const Point& direction: directions){
        newX = x + direction.first;
        newY = y + direction.second;

        // Check for valid Black
        if(isValid(newX, newY, m, n, visited, blocks, index)){
            result++;
            DFS(index, m, n, visited, blocks, result);
        }
    }
}

long long findLargestBlockSize(int m, int n, BlocksType& blocks){
    VisitedType visited(m, false);

    long long result = 0;
    long long current = 0;

    // Run DFS on each unvisited black block
    for(int i = 0; i < blocks.size(); i++){
        if(!visited[i]){
            current = 1;
            DFS(i, m, n, visited, blocks, current);
            result = max(result, current);
        }
    }

    return result;
}


int main(){
    int m, n, k;
    cin >> m >> n;
    cin >> k;
    BlocksType blocks(k);
    for(int i = 0; i < k; i++){
        cin >> blocks[i].first >> blocks[i].second;
    }
    sort(blocks.begin(), blocks.end());
    cout << findLargestBlockSize(m, n, blocks) << endl;

    return 0;
}


// test case
// 0 1 2 3 4 5 6 7
//
// 1 0 0 0 0 1 1 0    0 
// 0 1 1 1 1 1 1 1    1 
// 1 0 1 1 0 0 0 1    2
// 0 0 0 1 0 1 1 1    3
// 0 1 0 1 1 0 0 0    4
// 1 1 0 1 1 0 0 0    5

// 6 8
// 25
// 0 0
// 0 5
// 0 6
// 1 1
// 1 2
// 1 6
// 1 7
// 2 0
// 2 2
// 2 3
// 2 7
// 3 3
// 3 5
// 3 6
// 3 7
// 4 1
// 4 3
// 4 4
// 5 0
// 5 1
// 5 3
// 5 4
// 1 3
// 1 4
// 1 5

