#ifndef GRAPH_H
#define GRAPH_H
#include <QtCore>

 class graph {
    public: int vertexes[10];
    bool edges[20][20];
    int dim;
    graph();
    void link(int v, int u);
    QString toQString();
    int toInt(bool b);
    QList<int> getOutlinks (int v);
    QList<int> getInlinks (int v);
    void giveId(void);
    int getAnotherEnd(int idEdge, int idNode);
};

#endif // GRAPH_H
