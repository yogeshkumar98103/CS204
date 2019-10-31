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
            if(!q.empty()) q.push(-1);
            c = !c;
            continue;
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
    int u, v, w, dummyIndex = vertexCount;
    bool parity;
    for(int i = 0; i < edgeCount; i++){
        cin >> u >> v >> w;
        u--;
        v--;
        if(w%2 == 0){
            adjList.push_back(vector<int>());
            adjList[v].push_back(dummyIndex);
            adjList[u].push_back(dummyIndex);
            adjList[dummyIndex].push_back(v);
            adjList[dummyIndex].push_back(u);
            dummyIndex++;
        }
        else{
            adjList[v].push_back(u);
            adjList[u].push_back(v);
        }
    }

    if(checkBipartite(adjList, vertexCount)){
        cout << "No" << endl;
    }
    else{
        cout << "YES" << endl;
    }

    return 0;
}


/*

5 4
1 2 1
2 3 2
3 4 3
4 5 4
5 1 5

 */