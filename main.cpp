#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>
#include "graph.h"
#include "FordFulkerson.h"
#define VERSION_2
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

FordFulkerson ff;

int main(int argc, char** argv) {
    
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
    /*for (int i=0; i<5; i++) {
        trayecto tra;
        cin >> tra.origen;
        cin >> tra.destino;
        cin >> tra.salida;
        cin >> tra.llegada;
        T.insert(T.end(), tra);
    }*/
    // Odenar las entradas de acuerdo al tiempo de salida
    sort(T.begin(), T.end(), trayecto::comparar);
    
        
    // Crear el grafo
    graph grafo = graph(T.size()*2+4); // Los dos vertices de mas son para S y T
    //2 vertices por trayecto, s y t con k y -k y 2 mas para convertirlo a maxflow

    // Agregar aristas entre la llegada y la salida de cada trayecto
    for (int i=0; i<T.size(); i++) {
        grafo.add_edge(i, T.size()+i);
        grafo.set_capacity(i, T.size()+i, 1);
        grafo.set_flux(i, T.size()+i, 1);
    }
    
    // Agregar aristas entre la llegada y la salida de los trayectos que estan en la misma ciudad y cumplen el descanso
    // y entre la llegada de los dos
    for (int i=0; i<T.size(); i++) {
       for (int j=i+1; j<T.size(); j++) {
           if (T[i].destino == T[j].origen && T[i].llegada+15 <= T[j].salida) {
                grafo.add_edge(T.size()+i, j);
                grafo.set_capacity(T.size()+i, j, 1);
                grafo.set_flux(T.size()+i, j, 0);
#ifdef VERSION_2
                // Agregar una arista entre las dos llegadas con capacidad infinita
                // La capacidad podría ser igual a la suma de todas las aristas que llegan al viaje i, 
                // pero como no hay problema si es más grande usaremos el mayor número posible para
                // simplificar el calculo
                grafo.add_edge(T.size()+i, T.size()+j);
                grafo.set_capacity(T.size()+i, T.size()+j, 5);
#endif
            }
        }
    }

    //Agregar aristas de S a los origenes y de T a los destinos
    for (int i=0; i<T.size(); i++) {
        
        grafo.add_edge(T.size()*2, i);//de S a origenes
        grafo.set_capacity(T.size()*2, i, 1);
        grafo.set_flux(T.size()*2, i, 0);
        
        grafo.add_edge(T.size()+i, T.size()*2+1);//de destinos a t
        grafo.set_capacity(T.size()+i, T.size()*2+1, 1);
        grafo.set_flux(T.size()+i, T.size()*2+1, 0);
    }
    
    //Eliminar cotas inferiores del grafo
    for (int i = 0; i < grafo.get_size(); i++) {
        for (int j = 0; j < grafo.get_size(); j++) {
            if (grafo.has_edge(i, j)) {
                int cota = grafo.get_flux(i, j);
                if (cota != 0) {
                    //Flujo a cero
                    grafo.set_flux(i, j, 0);
                    //A la capacidad de la arista le restamos la cota
                    grafo.set_capacity(i, j, grafo.get_capacity(i, j)-cota);
                    //Al origen de la arista le sumamos la cota
                    grafo.set_value(i, grafo.get_value(i)+cota);
                    //Al destino de la arista le restamos la cota
                    grafo.set_value(j, grafo.get_value(j)-cota);
                }
            }
        }
    }
    // std::cout << "Cotas eliminadas" << std::endl;
    
    int k = 0;
    int maxflow = -1;
    while (maxflow == -1) {
        k++;
        std::cout << "K: " << k << std::endl;
        graph cGrafo = graph(grafo);
        //std::cout << "Aristas entre S, T y origenes y destinos anadidas" << std::endl;
        //S y T tienen de value -k y k
        cGrafo.set_value(T.size()*2, -k);
        cGrafo.set_value(T.size()*2+1, k);
        // std::cout << "Valores de S y T anadidos (K y -K)" << std::endl;
        
        //Eliminamos las capacidades de los nodos usando S y T
        int s = T.size()*2 + 2;
        int t = T.size()*2 + 3;
        for (int i = 0; i < s; i++) {
            int value = cGrafo.get_value(i);
            if (value < 0) {
                cGrafo.add_edge(s, i);
                cGrafo.set_capacity(s, i, -value);
            }
            else {
                cGrafo.add_edge(i, t);
                cGrafo.set_capacity(i, t, value);
            }
        }

        // std::cout << "Grafo listo" << std::endl;
        maxflow = ff.run(cGrafo, s, t);
        if (maxflow != -1) {
            grafo = graph(cGrafo);
        }
       // std::cout <<" MaxFlow: " << maxflow << std::endl;
    }
    std::cout << k << std::endl;
    // imprimimos el camino
    
    int s = T.size()*2;
    int t = T.size()*2+1;
    std::list<int> pilots = grafo.get_neighbours(s);
    for (std::list<int>::iterator it = pilots.begin(); it != pilots.end(); it++) {
        if (grafo.get_flux(s, *it) > 0) {
            int parent[grafo.get_size()];
            std::queue<int> Q;
            bool visited[grafo.get_size()];
            for (int i = 0; i < grafo.get_size(); ++i) {
                visited[i] = false;
            }
            Q.push(s);
            visited[s] = true;
            parent[s] = -1;
            while (!Q.empty()) {
                int node = Q.front();
                Q.pop();
                std::list<int> neighbours;
                neighbours = grafo.get_neighbours(node);

              //  std::cout << "NODE: " << node << std::endl;
                for (std::list<int>::iterator it = neighbours.begin(); it != neighbours.end(); it++) {
                    int u = *it;
                    //std::cout << "vecino: " << u << std::endl;
                    if (!visited[u] && (grafo.get_flux(node, u) == 1 || u == node+T.size())) {
                       // std::cout << std::endl << "Capacity arista " << node << " " << u << ": " << grafo.get_capacity(node, u) << std::endl;
                        visited[u] = true;
                        Q.push(u);
                        parent[u] = node;
                    }
                }
            }
            for (int node = t; node != s; ) {
                    int u = parent[node];
                    if (node < T.size()) {
                        std::cout << node << " ";
                    }
                    if (grafo.has_edge(u, node)) {
                        grafo.set_flux(u, node, 0);
                    }
                    node = u;
            }
            std::cout << std::endl;
        }
    }
    return 0;
}