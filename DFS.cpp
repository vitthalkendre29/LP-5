#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>
using namespace std;

void parallel_dfs(vector<vector<int>>& graph, int start) {
    int n = graph.size();
    vector<bool> visited(n, false);
    stack<int> s;
    s.push(start);
    visited[start] = true;
    cout << "DFS: ";
    #pragma omp parallel
    {
        #pragma omp single
        {
            while (!s.empty()) {
                int vertex;
                #pragma omp critical
                {
                    vertex = s.top();
                    s.pop();
                    cout << vertex << " ";
                }
                #pragma omp task firstprivate(vertex)
                {
                    for (int neighbor : graph[vertex]) {
                        #pragma omp critical
                        {
                            if (!visited[neighbor]) {
                                visited[neighbor] = true;
                                s.push(neighbor);
                            }
                        }
                    }
                }
            }
        }
    }
    cout << endl;
}

int main() {
    int n = 7;
    vector<vector<int>> graph(n);
    graph[0] = {1, 2};
    graph[1] = {0, 3, 4};
    graph[2] = {0, 5, 6};
    graph[3] = {1};
    graph[4] = {1};
    graph[5] = {2};
    graph[6] = {2};
    parallel_dfs(graph, 0);
    return 0;
}
