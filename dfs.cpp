#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

vector<vector<int>> adj;
vector<bool> visited;

void parallel_dfs(int node) {
    bool already_visited = false;
    #pragma omp critical
    {
        if (visited[node]) already_visited = true;
        else { visited[node] = true; cout << node << " "; }
    }
    if (already_visited) return;

    #pragma omp parallel for
    for (int i = 0; i < (int)adj[node].size(); i++) {
        if (!visited[adj[node][i]])
            parallel_dfs(adj[node][i]);
    }
}

int main() {
    int n, e, start;
    cout << "Enter number of nodes: "; cin >> n;
    cout << "Enter number of edges: "; cin >> e;
    adj.resize(n);
    visited.assign(n, false);
    cout << "Enter " << e << " edges (u v):\n";
    for (int i = 0; i < e; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    cout << "Enter starting node: "; cin >> start;
    cout << "\nDFS Traversal: ";
    parallel_dfs(start);
    cout << endl;
}