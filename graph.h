#ifndef GRAPH_H
#define	GRAPH_H

#include <list>
#include <vector>
#include <iostream>

/** Grafo.
 */
class graph {
public:
    /**
     * Constructor vacio.
     */
    graph();
    /**
     * Contructor.
     * @param size Cantidad de nodos
     */
    graph(int size);
    /**
     * Constructor de copia.
     * @param orig Objeto a copiar
     */
    graph(const graph& orig);
    /**
     * Cambia el valor del nodo id_node a value.
     * @param id_node Id del nodo
     * @param value Nuevo valor
     */
    void set_value(int id_node, int value);
    /**
     * Cambia la capacidad de la arista entre id_node_1 y id_node_2 a capacity.
     * @param id_node_1 Id del nodo 1
     * @param id_node_2 Id del nodo 2
     * @param capacity Nueva capacidad
     */
    void set_capacity(int id_node_1, int id_node_2, int capacity);
    /**
     * Cambia el flujo de la arista entre id_node_1 y id_node_2 a flux.
     * @param id_node_1 Id del nodo 1
     * @param id_node_2 Id del nodo 2
     * @param flux Nuevo flujo
     */
    void set_flux(int id_node_1, int id_node_2, int flux);
    /**
     * Devuelve el valor del nodo id_node.
     * @param id_node Id del nodo
     * @return Valor del nodo
     */
    int get_value(int id_node);
    /**
     * Devuelve la capacidad de la arista entre id_node_1 y id_node_2.
     * @param id_node_1 Id del nodo 1
     * @param id_node_2 Id del nodo 2
     * @return Capacidad de la arista
     */
    int get_capacity(int id_node_1, int id_node_2);
    /**
     * Devuelve el flujo de la arista entre id_node_1 y id_node_2.
     * @param id_node_1 Id del nodo 1
     * @param id_node_2 Id del nodo 2
     * @return Flujo de la arista
     */
    int get_flux(int id_node_1, int id_node_2);
    /**
     * Devuelve la cantidad de nodos del grafo.
     * @return Cantidad de nodos.
     */
    int get_size();
    /**
     * Agrega una arista entre id_node_1 y id_node_2.
     * @param id_node_1 Id del nodo 1
     * @param id_node_2 Id del nodo 2
     */
    void add_edge(int id_node_1, int id_node_2);
    /**
     * Devuelve el numero de nodos accesibles desde id_nodo.
     * @param id_node Id del nodo
     */
    int get_num_neighbours(int id_node);
    /**
     * Devuelve los nodos accesibles desde id_nodo.
     * @param id_node Id del nodo
     */
    std::list<int> get_neighbours(int id_node);
    /**
     * Devuelve los nodos que acceden a id_nodo.
     * @param id_node Id del nodo
     */
    std::list<int> get_parents(int id_node);
    /**
     * Devuelve True si existe una arista entre los dos nodos (de id_node_1 a id_nodo_2).
     * @param id_node_1 Id del nodo 1
     * @param id_node_2 Id del nodo 2
     */
    bool has_edge(int id_node_1, int id_node_2);
    
    void print() {
        std::cout << "vertices:\n";
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                std::cout << vertexs[i][j];
            }
            std::cout << "\n";
        }
        
        std::cout << "capacidades:\n";
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                std::cout << capacities[i][j];
            }
            std::cout << "\n";
        }
        
        std::cout << "flujos:\n";
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                std::cout << fluxes[i][j];
            }
            std::cout << "\n";
        }
    }
private:
    /**
     * Cantidad de nodos.
     */
    int size;
    /**
     * Flujo de las aristas y valor de los nodos.
     */
    std::vector<std::vector<int> > fluxes;
    /**
     * Capacidad de las aristas.
     */
    std::vector<std::vector<int> > capacities;
    /**
     * Vertices existentes
     */
    std::vector<std::vector<bool> > vertexs;
    /**
     * Listas de adyascencia, list_dest[v] contiene todos los nodos accesibles desde v
     */
    std::vector<std::list<int> > list_dest;
    /**
     * Listas de adyascencia, list_orig[v] contiene todos los nodos que llegan a v
     */
    std::vector<std::list<int> > list_orig;
};

#endif	/* GRAPH_H */

