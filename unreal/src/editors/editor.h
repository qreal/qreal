//=====================================================================
// Project:      unREAL
// File Name:    editor.h
// Description:  Editor 
//
// Created:      30-January-07
// Revision:      
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#ifndef EDITOR_H
#define EDITOR_H

#include <QString>
#include <QList>
#include <QMap>
#include <QAction>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>

class Element;
class Link;
class Property;

class Editor{
public:

    Editor(); // creates actions, nodes, edges, execs scripts (if needed)
    ~Editor(); // delete all ptr stuff 

    QMap<QString, Element*> nodes;
    QMap<QString, Link*> edges;
  
    QList<QAction*> actions;
    QList<QString> scripts;
  
private:
    void createActions();
    void createNodes();
    void createEdges();
    void execDBScripts();
};

class Element{
public:
    
    Element(){}
    ~Element(){ 
        qDeleteAll(properties.begin(), properties.end());
    }
 
    //graphics:
    QString svg; // path or shape itself

    //logic:
    QList<Property*> properties;
    QList<QString> links;
//    bool generalized;       // optional
//    QString generalization; 
};

class Link{
public:
    //graphics:
    QString svg;
  
    //logic:
    QList<QString> beginsWith;
    QList<QString> endsWith;
};

class Property{
public:
    QString name;
    QString type;
};

#endif 
