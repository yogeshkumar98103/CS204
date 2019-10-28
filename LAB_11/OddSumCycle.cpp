#include <iostream>
#include <queue>
#include <vector>
using namespace std;

typedef vector<vector<int>> AdjList_t;

bool bfs(const AdjList_t& adjList, vector<bool>& visited, vector<bool>& color, int start){
    queue<int> q;
    q.push(start);
    q.push(-1);
    visited[start] = true;

    int current;
    bool c = true;
    while(!q.empty()){
        current = q.front();
        q.pop();

        if(current == -1){
            if(q.size() > 1) q.push(-1);
            c = !c;
        }

        for(int to: adjList[current]){
            if(!visited[to]){
                color[to] = c;
                q.push(to);
            }
            else if(color[to] != c) return false;
            visited[to] = true;
        }
    }
    return true;
}

bool checkBipartite(const AdjList_t& adjList, int v){
    vector<bool> visited(v, false);
    vector<bool> color(v);
    for(int i = 0; i < v; i++){
        if(!visited[i] && !bfs(adjList, visited, color, i)) return false;
    }
    return true;
}

int main(){
    int vertexCount, edgeCount;
    cin >> vertexCount >> edgeCount;
    AdjList_t adjList(vertexCount);
    int u, v, w;
    bool parity;
    for(int i = 0; i < edgeCount; i++){
        cin >> u >> v >> w;
        u--;
        v--;
        parity = (w%2 == 0);
        adjList[v].push_back({u,parity});
        adjList[u].push_back({v,parity});
    }

    if(checkBipartite(adjList, vertexCount)){
        cout << "YES" << endl;
    }
    else{
        cout << "NO" << endl;
    }

    return 0;
}


/*

9 10
1 2
1 7
1 4
9 2
6 8
6 7
3 7
3 5
3 4
8 2

 */