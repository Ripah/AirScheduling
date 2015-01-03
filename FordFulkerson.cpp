/* 
 * File:   FordFulkerson.cpp
 * Author: Martin
 * 
 * Created on 30 de diciembre de 2014, 17:23
 */

#include "FordFulkerson.h"
#include <iostream>

FordFulkerson::FordFulkerson() {
}

FordFulkerson::FordFulkerson(const FordFulkerson& orig) {
}
int FordFulkerson::run(graph &grafo, int s, int t){
    //std::cout << "FF" << std::endl;
    BFS bfs;
    graph rGraph(grafo.get_size());
    for (int i = 0; i < rGraph.get_size(); ++i) {
        for (int j = 0; j < rGraph.get_size(); ++j) {
            //Look at all vertex pairs
            if (grafo.has_edge(i,j)) {
                int flux = grafo.get_flux(i,j);
                int capacity = grafo.get_capacity(i,j);
                int residual_capacity = capacity - flux;
                rGraph.add_edge(j, i);
                rGraph.add_edge(i, j);
                rGraph.set_capacity(i,j,residual_capacity);
                rGraph.set_capacity(j,i,flux);
            }
        }
    }
    //std::cout << "Residual Graph OK" << std::endl;
    int parent[grafo.get_size()];
    int max_flow = 0;
    while(bfs.search(rGraph, s, t, parent)) {
       // std::cout << "Hay camino" << std::endl;
        int flow = std::numeric_limits<int>::max();
        int u;
        for (int node = t; node != s; node = parent[node]) {
            u = parent[node];
            flow = std::min(flow, rGraph.get_capacity(u, node));
        }
        for (int node = t; node != s; node = parent[node]) {
            u = parent[node];
            if (grafo.has_edge(u, node)){
                rGraph.set_capacity(u, node, rGraph.get_capacity(u, node)-flow);
                grafo.set_flux(u, node, grafo.get_flux(u, node)+flow);
            }
            else {
                rGraph.set_capacity(node, u, rGraph.get_capacity(node, u)+flow);
                grafo.set_flux(node, u, grafo.get_flux(node, u)-flow);
            }
        }
    }
    int sat = 0;
    std::list<int> vecinos_s = grafo.get_neighbours(s);
    for (std::list<int>::iterator it = vecinos_s.begin(); it != vecinos_s.end(); it++) {
        max_flow += grafo.get_flux(s, (*it));
        sat += grafo.get_capacity(s, (*it));
    }
    std::cout << "Capacidad: " << sat << std::endl;
    std::cout << "Flujo: " << max_flow << std::endl;
    if (max_flow == sat) {
        return max_flow;
    }
    else return -1;
}
FordFulkerson::~FordFulkerson() {
}

