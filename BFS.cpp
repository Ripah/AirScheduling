/* 
 * File:   BFS.cpp
 * Author: Martin
 * 
 * Created on 30 de diciembre de 2014, 17:04
 */

#include "BFS.h"
#include <iostream>
BFS::BFS() {
}

BFS::BFS(const BFS& orig) {
}
bool BFS::search(graph grafo, int s, int t, int parent[]) {
    //std::cout << "BFS" << std::endl;
    std::queue<int> Q;
    bool visited[grafo.get_size()];
    for (int i = 0; i < grafo.get_size(); ++i) {
        visited[i] = false;
    }
    Q.push(s);
    visited[s] = true;
    parent[s] = -1;
    while (!Q.empty() && !visited[t]) {
        int node = Q.front();
        Q.pop();
        std::list<int> neighbours = grafo.get_neighbours(node);
        //std::cout << "NODE: " << node << std::endl;
        for (std::list<int>::iterator it = neighbours.begin(); it != neighbours.end(); it++) {
            int u = *it;
            //std::cout << "vecino: " << u << std::endl;
            if (!visited[u] && grafo.get_capacity(node, u) != 0) {
                //std::cout << std::endl << "Capacity arista " << node << " " << u << ": " << grafo.get_capacity(node, u) << std::endl;
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


