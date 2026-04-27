#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

// ─── BFS ───────────────────────────────────────────
void parallel_bfs(vector<vector<int>>& adj, int start, int n) {
    vector<bool> visited(n, false);
    queue<int> q;
    visited[start] = true;
    q.push(start);
    cout << "\nBFS Traversal: ";
    while (!q.empty()) {
        int size = q.size();
        vector<int> level;
        for (int i = 0; i < size; i++) {
            level.push_back(q.front());
            q.pop();
        }
        #pragma omp parallel for
        for (int i = 0; i < (int)level.size(); i++) {
            int node = level[i];
            #pragma omp critical
            cout << node << " ";
            for (int nb : adj[node]) {
                #pragma omp critical
                if (!visited[nb]) { visited[nb] = true; q.push(nb); }
            }
        }
    }
    cout << endl;
}

// ─── DFS ───────────────────────────────────────────
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

// ─── MAIN ──────────────────────────────────────────
int main() {
    int n, e, start;

    cout << "Enter number of nodes: "; cin >> n;
    cout << "Enter number of edges: "; cin >> e;

    adj.resize(n);
    visited.assign(n, false);
    vector<vector<int>> adj_bfs(n);

    cout << "Enter " << e << " edges (u v):\n";
    for (int i = 0; i < e; i++) {
        int u, v; cin >> u >> v;
        adj_bfs[u].push_back(v);
        adj_bfs[v].push_back(u);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    cout << "Enter starting node: "; cin >> start;

    // BFS
    parallel_bfs(adj_bfs, start, n);

    // Reset visited for DFS
    fill(visited.begin(), visited.end(), false);

    // DFS
    cout << "DFS Traversal: ";
    parallel_dfs(start);
    cout << endl;

    return 0;
}