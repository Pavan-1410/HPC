#include <iostream> 
#include <vector> 
#include <queue> 
#include <omp.h> 
using namespace std; 
 
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

int main() { 
    int n, e, start; 
    cout << "Enter number of nodes: "; cin >> n; 
    cout << "Enter number of edges: "; cin >> e; 
    vector<vector<int>> adj(n); 
    cout << "Enter " << e << " edges (u v):\n"; 
    for (int i = 0; i < e; i++) { 
        int u, v; cin >> u >> v; 
        adj[u].push_back(v); 
        adj[v].push_back(u); 
    } 
    cout << "Enter starting node: "; cin >> start; 
    parallel_bfs(adj, start, n); 
}