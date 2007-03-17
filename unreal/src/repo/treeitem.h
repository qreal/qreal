//=====================================================================
// Project:      unREAL 
// File Name:    treeitem.h
// Description:  Tree item, provides hierarchical structures for 
//               relational tables 
//
// Created:      16-Jan-07
// Revision:     17-Mar-07 
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QStringList>

class TreeItem{

public:
    TreeItem( QStringList _vals, QMap<QString, QString>*, TreeItem *parent, QSqlDatabase _db);
    ~TreeItem();

    int rowCount() const;
    int columnCount() const { return 1; }
    int childCount() const { return childItems.size(); }
    
    QVariant data( int i ) const;
    int getID() const { return id; }
    QString getType() const { return values.at(1); }                          
    QString getName() const { return values.at(0); }
    QString getDiagramName() const { return values.at(2); }
    QString getBeginning() const { if (values.size() >= 4) return values.at(3); }
    QString getEnding() const { if (values.size() >= 5) return values.at(4); }
    TreeItem* getChild( QString );
    TreeItem* getChild( int );
    
    void setName( QString n ) { values.replace(0, n); }
    void setType( QString t ) { values.replace(1, t); }
    void setEnds( QString, QString );
    void setData( QString d ) { values.replace(0, d); }
    void setDiagramName( QString d ) { values.replace(2, d); }
    void setID( int _id ) { id = _id; }
    
    void addChild( TreeItem* );  
    void removeChild ( QString );
    
    TreeItem *parent() { return parentItem; }
    
    int row();
    
    bool isTable();
    
//private:
    int id;
    //QString name;
    //QString type;
    //QString diagramName;
    //QString beginsWith;
    //QString endsWith;
    QList<TreeItem*> childItems;
    TreeItem *parentItem;
    QMap<QString, QString> *tables;
    QSqlDatabase db;
    QStringList values;   
    /* 0 - name, 
       1 - type, 
       2 - diagram, 
       3,4,5 etc. - as in DB create table script:
             "description, priority, source, status" for nodes or
             "beginsWith, endsWith, status" for edges
    */
};

#endif
