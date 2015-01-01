/* 
 * File:   BFS.cpp
 * Author: Martin
 * 
 * Created on 30 de diciembre de 2014, 17:04
 */

#include "BFS.h"

BFS::BFS() {
}

BFS::BFS(const BFS& orig) {
}
bool BFS::search(const graph &grafo, int s, int t, int &parent[]) {
    std::queue<int> Q;
    bool visited[grafo.get_size()];
    Q.push(s);
    visited[s] = true;
    parent[s] = -1;
    while (!Q.empty()) {
        int node = Q.front();
        Q.pop();
        std::list<int> neighbours = grafo.get_neighbours(node);
        for (std::list<int>::iterator it = neighbours.begin(); it != neighbours.end(); it++) {
            int u = *it;
            if (!visited[u] && grafo.get_capacity(node, u) > 0) {
                visited[u] = true;
                Q.push(u);
                parent[u] = node;
            }
        }
    }
    return (visited[t] == true);
}
BFS::~BFS() {
}

