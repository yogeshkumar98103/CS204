#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

typedef pair<int, int>                  Point;
typedef vector<vector<bool>>            BlockMatricType;

static const vector<Point> directions = {{1,0},{-1,0},{0,1},{0,-1}};

inline bool isValid(int x, int y, const int m, const int n, BlockMatricType& visited, const BlockMatricType& blocks){
    return (x >= 0 && x < m) && (y >= 0 && y < n) && (!visited[x][y]) && blocks[x][y];
}

void DFS(int x, int y, const int m, const int n, BlockMatricType& visited, const BlockMatricType& blocks, long long &result){
    visited[x][y] = true;
    int newX, newY;

    // Look in all directions
    for(const Point& direction: directions){
        newX = x + direction.first;
        newY = y + direction.second;

        // Check for valid Black
        if(isValid(newX, newY, m, n, visited, blocks)){
            result++;
            DFS(newX, newY, m, n, visited, blocks, result);
        }
    }
}

long long findLargestBlockSize(int m, int n, const vector<Point>& blackBlocks){
    BlockMatricType visited(m, vector<bool> (n,false));
    BlockMatricType blocks(m, vector<bool> (n,false));

    long long result = 0;
    long long current = 0;

    // Colour blocks
    for(const Point& b : blackBlocks){
        blocks[b.first][b.second] = true;
    }

    // Run DFS on each unvisited black block
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(!visited[i][j] && blocks[i][j]){
                current = 0;
                DFS(i, j, m, n, visited, blocks, current);
                result = max(result, current);
            }
        }
    }

    return result;
}


int main(){
    int t, m, n, k;
    cin >> t;
    while(t--){
        cin >> m >> n;
        cin >> k;
        vector<Point> blackBlocks(k);
        for(int i = 0; i < k; i++){
            cin >> blackBlocks[i];
        }
        findLargestBlockSize(m, n, blackBlocks);
    }

    return 0;
}