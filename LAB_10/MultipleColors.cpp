#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

typedef pair<int, int>                  Point;
typedef vector<vector<int>>             BlockMatricType;
typedef vector<vector<bool>>            VisitedType;

static const vector<Point> directions = {{1,0},{-1,0},{0,1},{0,-1}};

inline bool isValid(int x, int y, const int m, const int n, int color, VisitedType& visited, const BlockMatricType& blocks){
    return (x >= 0 && x < m) && (y >= 0 && y < n) && (!visited[x][y]) && (blocks[x][y] == color);
}

void DFS(int x, int y, const int m, const int n, VisitedType& visited, const BlockMatricType& blocks, long long &result){
    visited[x][y] = true;
    int newX, newY;

    // Look in all directions
    for(const Point& direction: directions){
        newX = x + direction.first;
        newY = y + direction.second;

        // Check for valid Black
        if(isValid(newX, newY, m, n, blocks[x][y], visited, blocks)){
            result++;
            DFS(newX, newY, m, n, visited, blocks, result);
        }
    }
}

long long findLargestBlockSize(int m, int n, BlockMatricType& blocks){
    VisitedType visited(m, vector<bool> (n,false));

    long long result = 0;
    long long current = 0;

    // Run DFS on each unvisited black block
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(!visited[i][j]){
                current = 1;
                DFS(i, j, m, n, visited, blocks, current);
                result = max(result, current);
            }
        }
    }

    return result;
}


int main(){
    int m, n, c;

    cin >> m >> n;
    cin >> c;
    BlockMatricType blocks(m, vector<int> (n,0));
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            cin >> blocks[i][j];
        }
    }
    cout << findLargestBlockSize(m, n, blocks) << endl;

    return 0;
}

// 6 8
// 5
// 1 4 4 4 4 3 3 1
// 2 1 1 4 3 3 1 1
// 3 2 1 1 2 3 2 1
// 3 3 2 1 2 2 2 2
// 3 1 3 1 1 4 4 4
// 1 1 3 1 1 4 4 4

// 6 8
// 2
// 1 0 0 0 0 1 1 0 
// 0 1 1 1 1 1 1 1 
// 1 0 1 1 0 0 0 1
// 0 0 0 1 0 1 1 1
// 0 1 0 1 1 0 0 0 
// 1 1 0 1 1 0 0 0