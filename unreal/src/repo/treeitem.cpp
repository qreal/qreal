//=====================================================================
// Project:      unREAL 
// File Name:    treeitem.cpp
// Description:  Tree item, provides hierarchical structures for 
//               relational tables 
//
// Created:      16-Jan-07
// Revision:     21-Mar-07 
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#include <QStringList>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QtGui>
//#define _LONG_DEBUG
#include "dbg.h"
          
#include "treeitem.h"

TreeItem::TreeItem(QStringList _vals, QMap<QString, QString> *map, TreeItem *parent, QSqlDatabase _db)
{dbg;
//qDebug() << "TREEITEM: "<< _vals;
    db = _db;
    parentItem = parent;
    tables = map;
    for ( int i=0; i<13; i++){
        if (i<_vals.size())
            values << _vals.at(i);
        else    
            values << "";    
    }        
}

TreeItem::~TreeItem(){
dbg;
  qDeleteAll(childItems);
}

int TreeItem::rowCount() const{
dbg; 
    if(!tables->contains(values.at(1)))
        return 0;
  
    QSqlQuery qq = db.exec((*tables)[values.at(1)]);
    bool a = db.driver()->hasFeature(QSqlDriver::QuerySize);
    if (a){
        return qq.size();
    }    
    else{ // really horrible way, but nothing else to do if size() is not supported
        qq.last();
        return qq.at() + 1;
    }  
}

int TreeItem::columnCount() const {
dbg;
    //if( values.at(1) == "diagram" )
    return 10; 
}

void TreeItem::addChild(TreeItem* item){
dbg;
    childItems.append(item);   
}

TreeItem* TreeItem::getChild(QString str){
dbg;  
    int i=0;
    while (i<childItems.size() && childItems.at(i)->values.at(1) != str)
        i++;
    if(i == childItems.size())
        return 0;
    else 
        return childItems.value(i);
}

TreeItem* TreeItem::getChild( int pos ){
dbg;
    if (childItems.size() == 0)
        return 0;
    if (pos < childItems.size() && pos >= 0){
        return childItems.value(pos);
    }    
    else {
        return 0 ;
    }    
}

int TreeItem::row(){
dbg;
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
    return 0;  
}

bool TreeItem::isTable(){
dbg;
    return tables->contains(values.at(1));
}
                                                     
void TreeItem::removeChild( QString name ){
dbg;
    int i=0;
    while (i < childItems.size() && childItems.at(i)->values.at(1) != name)
        i++;
    if (childItems.at(i)->values.at(1) == name){
        delete childItems.at(i);
        childItems.removeAt(i);
    }
}

void TreeItem::removeChild(int pos){
dbg;
    delete childItems.at(pos);
    childItems.removeAt(pos);
}

void TreeItem::setEnds( QString b, QString e){
dbg;
    values.replace(7, b); // beginsWith
    values.replace(8, e); // endsWith    
}

QVariant TreeItem::data( int i ) const { 
dbg;
    if (i<=values.size())
        return values.at(i);
     return QVariant();   
}
