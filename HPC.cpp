#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
 
using namespace std;
 
void parallelBFS(vector<vector<int>>& graph, int start) {
    int n = graph.size();
 
    vector<bool> visited(n, false);
    queue<int> q;
 
    visited[start] = true;
    q.push(start);
 
    while (!q.empty()) {
        int size = q.size();
        vector<int> currentLevel;
 
        // Store current level nodes
        for (int i = 0; i < size; i++) {
            int node = q.front();
            q.pop();
            currentLevel.push_back(node);
        }
 
        #pragma omp parallel for
        for (int i = 0; i < currentLevel.size(); i++) {
            int node = currentLevel[i];
 
            #pragma omp critical
            cout << node << " ";
 
            for (int neighbor : graph[node]) {
                if (!visited[neighbor]) {
 
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
    }
}

void dfsUtil(vector<vector<int>>& graph, int node, vector<bool>& visited) {
 
    #pragma omp critical
    {
        if (visited[node]) return;
        visited[node] = true;
        cout << node << " ";
    }
 
    #pragma omp parallel for
    for (int i = 0; i < graph[node].size(); i++) {
        int neighbor = graph[node][i];
 
        if (!visited[neighbor]) {
            dfsUtil(graph, neighbor, visited);
        }
    }
}
 
void parallelDFS(vector<vector<int>>& graph, int start) {
    int n = graph.size();
    vector<bool> visited(n, false);
 
    dfsUtil(graph, start, visited);
}
 
 
int main() {
    int n = 6;
    vector<vector<int>> graph(n);
 
    graph[0] = {1, 2};
    graph[1] = {0, 3, 4};
    graph[2] = {0, 5};
    graph[3] = {1};
    graph[4] = {1};
    graph[5] = {2};
 
    cout << "Parallel BFS Traversal: ";
    parallelBFS(graph, 0);
    cout << "Parallel DFS Traversal: ";
    parallelDFS(graph, 0);
 
    return 0;
}
