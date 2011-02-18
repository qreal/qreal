#include "graph.h"
#include <QtCore>

void graph::giveId(void){
    int num = 0;
    for (int i=0; i<dim ; i++) {
        for (int j=0; j<dim; j++) {
            if(edges[i][j]) num++;
        }
    }
}

graph::graph() {
    dim=20;
    for (int i=0; i<dim ; i++) {
        for (int j=0; j<dim; j++) {
            edges[i][j] = false;
        }
    }
}

void graph::link(int v, int u) {
if (v >= dim || u >= dim || v < 0 || u < 0) {
qDebug()<<"Error!!!";
return;
}
edges[v][u]=true;
}

QList<int> graph::getOutlinks (int v) {
    QList<int> outs;
    if (v >= dim || v < 0) {
        qDebug()<<"Error!!!";
        return outs;
    }

    int id=0;
    for( int i = 0; i< dim; i++){
        for(int j=0; j<dim; j++){
            if(edges[i][j]){
                id++;
                if(i == v){
                    outs<<id;
                }
            }
        }
    }
    /*
    for (int i=0; i<dim; i++) {
        if (edges[v][i]) {
            outs<<i;
        }
    }*/
    return outs;
}

QList<int> graph::getInlinks (int v) {
    QList<int> ins;
    if (v >= dim || v < 0) {
        qDebug()<<"Error!!!";
        return ins;
    }


    int id=0;
    for( int i = 0; i< dim; i++){
        for(int j=0; j<dim; j++){
            if(edges[i][j]){
                id++;
                if(j == v){
                    ins<<id;
                }
            }
        }
    }
    return ins;
}

int graph::getAnotherEnd(int idEdge, int idNode){
    int id=0, res=-1;
    for( int i = 0; i< dim; i++){
        for(int j=0; j<dim; j++){
            if(edges[i][j]){
             /*   qDebug()<<i;
                qDebug()<<j;
                qDebug()<<id;
               */ id++;
                if(id == idEdge ){
                    if(i == idNode){
                        res = j;
              //          qDebug()<<"YES!";
                    }else if(j == idNode){
                        res= i;
           //             qDebug()<<"YES!";
                    } else qDebug()<<"Error!";
                }
            //    qDebug()<<"\n";
            }
        }
    }
return res;
}

QString graph::toQString() {
    QString result;
    result += " ";

    for (int i=0; i<dim; i++) {
        QString str;
        str.setNum(i);
        result += " ";

        result += str + " ";
    }
    result += "\n \n" ;
    for (int i=0; i<dim; i++) {
        QString str;
        str.setNum(i);

        result += str + " ";
        for (int j=0; j<dim; j++) {
            QString str;
            str.setNum(toInt(edges[i][j]));

            result += " ";
            result += str + " ";
        }
    result += "\n";
 }
result += " \n";
return result;
}

int graph::toInt(bool b) {
return b ? 1 : 0;
}



