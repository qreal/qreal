//=====================================================================
// Project:      unREAL 
// File Name:    treeitem.h
// Description:  Tree item, provides hierarchical structures for 
//               relational tables 
//
// Created:      16-Jan-07
// Revision:     21-Mar-07 
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
    int columnCount() const;
    int childCount() const { return childItems.size(); }
    
    QVariant data( int i ) const;
    int getID() const { return values.at(0); }
    QString getType() const { return values.at(2); }                          
    QString getName() const { return values.at(1); }
    QString getDiagramName() const { return values.at(3); }
    QString getBeginning() const { if (values.size() >= 6) return values.at(5); else return QString(-1); }
    QString getEnding() const { if (values.size() >= 7) return values.at(6); else return QString(-1); }
    TreeItem* getChild( QString );
    TreeItem* getChild( int );
    
    void setName( QString n ) { values.replace(1, n); }
    void setType( QString t ) { values.replace(2, t); }
    void setEnds( QString, QString );
    void setData( int i, QString d ) { values.replace(1, d); }
    void setDiagramName( QString d ) { values.replace(3, d); }
    void setID( int _id ) { values.replace(0,_id); }
    
    int getX() { return values.at(values.size()-2).toInt(); }
    int getY() { return values.at(values.size()-1).toInt(); }
    void setX( int x ) { values.replace(values.size()-2, QString(x)); }
    void setY( int y ) { values.replace(values.size()-1, QString(y)); }
    
    void addChild( TreeItem* );  
    void removeChild ( QString );
    
    TreeItem *parent() { return parentItem; }
    
    int row();
    
    bool isTable();
    
//private:
    int id;
    QList<TreeItem*> childItems;
    TreeItem *parentItem;
    QMap<QString, QString> *tables;
    QSqlDatabase db;
    QStringList values;   
    /* 0 -- uuid
     * 1 -- name, 
       2 -- type, 
       3 -- diagram, 
       4 -- status,
       5,6 etc. - as in DB create table script:
             "description, priority, source" for nodes or
             "beginsWith, endsWith" for edges
    */
};

#endif
