/* 
 * File:   FordFulkerson.h
 * Author: Martin
 *
 * Created on 30 de diciembre de 2014, 17:23
 */

#ifndef FORDFULKERSON_H
#define	FORDFULKERSON_H
#include "BFS.h"
#include <limits>
#include <algorithm>
class FordFulkerson {
public:
    FordFulkerson();
    FordFulkerson(const FordFulkerson& orig);
    virtual ~FordFulkerson();
    int run(graph &grafo, int s, int t);
private:

};

#endif	/* FORDFULKERSON_H */

