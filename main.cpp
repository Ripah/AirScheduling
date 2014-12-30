#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>
#include "graph.h"

using namespace std;

struct trayecto {
    int origen;
    int destino;
    int salida;
    int llegada;
    bool static comparar (trayecto i,trayecto j) {
        return i.salida < j.salida; 
    }
};

int main(int argc, char** argv) {
    // version 1
    // leer entrada 
    vector<trayecto> T;
    int num;
    while(cin >> num) {
        trayecto tra;
        tra.origen = num;
        cin >> tra.destino;
        cin >> tra.salida;
        cin >> tra.llegada;
        T.insert(T.end(), tra);
    }
    // Odenar las entradas de acuerdo al tiempo de salida
    sort(T.begin(), T.end(), trayecto::comparar);
    
    // Crear el grafo
    graph grafo = graph(T.size()*2+2); // Los dos vertices de mas son para S y T
    
    // Agregar aristas entre la llegada y la salida de cada trayecto
    for (int i=0; i<T.size(); i++) {
        grafo.add_edge(i, T.size()+i);
        grafo.set_capacity(i, T.size()+i, 1);
        grafo.set_flux(i, T.size()+i, 1);
    }
    
    // Agregar aristas entre la llegada y la salida de los trayectos que estan en la misma ciudad y cumplen el descanso
    for (int i=0; i<T.size(); i++) {
        for (int j=i+1; j<T.size(); j++) {
            if (T[i].destino == T[j].origen && T[i].llegada+15 <= T[j].salida) {
                grafo.add_edge(T.size()+i, j);
                grafo.set_capacity(T.size()+i, j, 1);
            }
        }
    }
    
    return 0;
}

