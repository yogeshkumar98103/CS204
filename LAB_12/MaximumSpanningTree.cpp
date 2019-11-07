#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

static const long long int MOD = (long long)(1e9 + 7);

class DisjointSet{
    vector<int> parent, rank;

public:

    explicit DisjointSet(int n){
        parent.resize(n);
        rank.resize(n);
    }

    void make_set(int ele){
        parent[ele] = ele;
        rank[ele] = 0;
    }

    int find_set(int ele){
        if(ele != parent[ele])
            parent[ele] = find_set(parent[ele]);
        return parent[ele];
    }

    void union_sets(int ele1, int ele2){
        int parent_s1 = find_set(ele1);
        int parent_s2 = find_set(ele2);

        if(rank[parent_s1] < rank[parent_s2])
            parent[parent_s1] = parent_s2;
        else if(rank[parent_s1] > rank[parent_s2])
            parent[parent_s2] = parent_s1;
        else{
            parent[parent_s2] = parent_s1;
            rank[parent_s1]++;
        }
    }
};  

struct Edge{
	int from;
	int to;
	long long weight;
};

bool compFn(const Edge& e1, const Edge& e2){
	return e1.weight >= e2.weight;
}

long long int find_maximum_spanning_tree(vector<Edge>& edges, int vertex_count){
    DisjointSet ds(vertex_count + 1);
    for(int i = 1; i <= vertex_count; ++i){
		ds.make_set(i);
	}
    sort(edges.begin(), edges.end(), compFn);
    long long int ans = 1;
	for(const Edge& e: edges){
		if(ds.find_set(e.from) != ds.find_set(e.to)){
			ds.union_sets(e.from, e.to);
			ans *= e.weight;
            ans %= MOD;
		}
	}
    return ans;
}

int main(){
	vector<Edge> edges;
	int vertex_count, edge_count;
	cin >> vertex_count >> edge_count;

	int u, v;
	long long int w;
	for (int i = 0; i < edge_count; ++i){
		cin >> u >> v >> w;
		edges.push_back({u,v,w});
	}

    long long int ans = find_maximum_spanning_tree(edges, vertex_count);
	cout << ans << endl;

	return 0;
}

/*

4 6
1 2 5
3 4 9
1 4 6
3 2 8
2 4 10
1 3 7 

*/