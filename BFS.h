/* 
 * File:   BFS.h
 * Author: Martin
 *
 * Created on 30 de diciembre de 2014, 17:04
 */

#ifndef BFS_H
#define	BFS_H
#include "graph.h"
#include <queue>

class BFS {
public:
    BFS();
    BFS(const BFS& orig);
    virtual ~BFS();
    bool search(const graph &grafo, int s, int t, int &parent[]);
private:

};

#endif	/* BFS_H */

