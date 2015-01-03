#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>
#include "graph.h"
#include "FordFulkerson.h"
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
    graph grafo = graph(T.size()*2+4); // Los dos vertices de mas son para S y T
    //2 vertices por trayecto, s y t con k y -k y 2 mas para convertirlo a maxflow

    // Agregar aristas entre la llegada y la salida de cada trayecto
    for (int i=0; i<T.size(); i++) {
        grafo.add_edge(i, T.size()+i);
        grafo.set_capacity(i, T.size()+i, 1);
        grafo.set_flux(i, T.size()+i, 1);
    }
    // std::cout << "Aristas entre llegada y salida anadidas" << std::endl;
    // Agregar aristas entre la llegada y la salida de los trayectos que estan en la misma ciudad y cumplen el descanso
    for (int i=0; i<T.size(); i++) {
       for (int j=i+1; j<T.size(); j++) {
           if (T[i].destino == T[j].origen && T[i].llegada+15 <= T[j].salida) {
                grafo.add_edge(T.size()+i, j);
                grafo.set_capacity(T.size()+i, j, 1);
                grafo.set_flux(T.size()+i, j, 0);
            }
        }
    }
    //std::cout << "Aristas entre transbordos anadidas" << std::endl;
    //Agregar aristas de S a los origenes y de T a los destinos
    for (int i=0; i<T.size(); i++) {
        
        grafo.add_edge(T.size()*2, i);//de S a origenes
        grafo.add_edge(T.size()+i, T.size()*2+1);//de destinos a t
        grafo.set_capacity(T.size()*2, i, 1);
        grafo.set_flux(T.size()*2, i, 0);
        grafo.set_capacity(T.size()+i, T.size()*2+1, 1);
        grafo.set_flux(T.size()+i, T.size()*2+1, 0);
    }
    
    int k = 1;
    int maxflow = -1;
    while (maxflow == -1) {
        std::cout << "K: " << k << std::endl;
        graph cGrafo = graph(grafo);
        //std::cout << "Aristas entre S, T y origenes y destinos anadidas" << std::endl;
        //S y T tienen de value -k y k
        cGrafo.set_value(T.size()*2, -k);
        cGrafo.set_value(T.size()*2+1, k);

       // std::cout << "Valores de S y T anadidos (K y -K)" << std::endl;
        //Eliminar cotas inferiores del grafo
        for (int i = 0; i < cGrafo.get_size(); i++) {
            for (int j = 0; j < cGrafo.get_size(); j++) {
                if (cGrafo.has_edge(i, j)) {
                    int cota = cGrafo.get_flux(i, j);
                    //A la capacidad de la arista le restamos la cota
                    cGrafo.set_capacity(i, j, cGrafo.get_capacity(i, j)-cota);
                    //Al origen de la arista le sumamos la cota
                    cGrafo.set_value(i, cGrafo.get_value(i)+cota);
                    //Al destino de la arista le restamos la cota
                    cGrafo.set_value(j, cGrafo.get_value(j)-cota);
                }
            }
        }
       // std::cout << "Cotas eliminadas" << std::endl;
        //Eliminamos las capacidades de los nodos usando S y T
        int s = T.size()*2+2;
        int t = T.size()*2 + 3;
        for (int i = 0; i < s; i++) {
            int value = cGrafo.get_value(i);
            if (value < 0) {
                cGrafo.add_edge(s, i);
                cGrafo.set_capacity(s, i, value*(-1));
            }
            else {
                cGrafo.add_edge(i, t);
                cGrafo.set_capacity(i, t, value);
            }
        }
       // std::cout << "Grafo listo" << std::endl;
        maxflow = ff.run(cGrafo, s, t);
        
        ++k;
       // std::cout <<" MaxFlow: " << maxflow << std::endl;
    }
    std::cout << "K: " << k << std::endl;
    return 0;
}

