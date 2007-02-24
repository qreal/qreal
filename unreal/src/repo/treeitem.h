//=====================================================================
// Project:      unREAL 
// File Name:    treeitem.h
// Description:  Tree item, provides hierarchical structures for 
//               relational tables 
//
// Created:      16-Jan-07
// Revision:     23-Feb-07 
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
    TreeItem( QString _name, QString _type, QString _diagram, QMap<QString, QString>*, TreeItem *parent, QSqlDatabase _db);
    ~TreeItem();

    int rowCount() const;
    int columnCount() const { return 1; }
    int childCount() const { return childItems.size(); }
    
    QVariant data( int i ) const { if (i!=1) return name; else return diagramName; }
    QString getType() const { return type; }
    QString getName() const { return name; }
    QString getDiagramName() const { return diagramName; }
    QString getBeginning() const { return beginsWith; }
    QString getEnding() const { return endsWith; }
    TreeItem* getChild( QString );
    TreeItem* getChild( int );
    
    void setName( QString n ) { name = n; }
    void setType( QString t ) { type = t; }
    void setEnds( QString, QString );
    void setData( QString d ) { name = d; }
    
    void addChild( TreeItem* );  
    void removeChild ( QString );
    
    TreeItem *parent() { return parentItem; }
    
    int row();
    
    bool isTable();
    
private:
    QString name;
    QString type;
    QString diagramName;
    QString beginsWith;
    QString endsWith;
    QList<TreeItem*> childItems;
    TreeItem *parentItem;
    QMap<QString, QString> *tables;
    QSqlDatabase db;
};

#endif
