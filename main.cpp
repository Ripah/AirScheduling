#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>
#include "graph.h"
#include "FordFulkerson.h"
#define VERSION_2
using namespace std;

struct trayecto {
    int id;
    int origen;
    int destino;
    int salida;
    int llegada;
    bool static comparar (trayecto i,trayecto j) {
        return i.salida < j.salida; 
    }
};

vector<trayecto> T;

void DFS(graph grafo, int node, int t) {
    // caso base
    if (node == t)
        return;
    // caso recursivo
    std::list<int> list = grafo.get_neighbours(node);
    std::list<int>::iterator it = list.begin();
    while (grafo.get_flux(node, *it) <= 0) it++;
    if (*it < T.size()) cout << " " << T[*it].id;
    grafo.set_flux(node, *it, grafo.get_flux(node, *it) -1);
    DFS(grafo, *it, t);
}

int main(int argc, char** argv) {
    
    // leer entrada 
    
    int num;
    int i = 1;
    while(cin >> num) {
        trayecto tra;
        tra.id = i; 
        tra.origen = num;
        cin >> tra.destino;
        cin >> tra.salida;
        cin >> tra.llegada;
        T.insert(T.end(), tra);
        i++;
    }

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
                grafo.set_capacity(T.size()+i, T.size()+j, std::numeric_limits<int>::max());
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
    FordFulkerson ff;
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

    int s = T.size()*2 + 2;
    int t = T.size()*2 + 3;
    // Flujos desde y hacia S y T fuera
    std::list<int> list = grafo.get_neighbours(s);
    for (std::list<int>::iterator it = list.begin(); it != list.end(); it++) {
        grafo.set_flux(s, *it, 0);
    }
    list = grafo.get_parents(t);
    for (std::list<int>::iterator it = list.begin(); it != list.end(); it++) {
        grafo.set_flux(*it, t, 0);
    }
    
    s = T.size()*2;
    t = T.size()*2+1;
    
    // Poner a 1 el flujo entre origen y destino de los viajes
    for (int i=0; i<T.size(); i++) {
        grafo.set_flux(i, T.size()+i, 1);
    }
    
    // DFS desde s para buscar caminos
    list = grafo.get_neighbours(s);
    for (std::list<int>::iterator it = list.begin(); it != list.end(); it++) {
        if (grafo.get_flux(s, *it) > 0) {
            cout << T[*it].id;
            DFS(grafo, *it, t);
            cout << endl;
        }
    }
    return 0;
}