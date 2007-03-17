//=====================================================================
// Project:      unREAL 
// File Name:    treeitem.cpp
// Description:  Tree item, provides hierarchical structures for 
//               relational tables 
//
// Created:      16-Jan-07
// Revision:     17-Mar-07 
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

TreeItem::TreeItem(QStringList _vals, QMap<QString, QString> *map, TreeItem *parent, QSqlDatabase _db){
dbg;
    db = _db;
    parentItem = parent;
    tables = map;
    for ( int i=0; i<10; i++){
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
    if(!tables->contains(values.at(0)))
        return 0;
  
    QSqlQuery qq = db.exec((*tables)[values.at(0)]);
    bool a = db.driver()->hasFeature(QSqlDriver::QuerySize);
    if (a){
        return qq.size();
    }    
    else{ // really horrible way, but nothing else to do if size() is not supported
        qq.last();
        return qq.at() + 1;
    }  
}

void TreeItem::addChild(TreeItem* item){
dbg;
    childItems.append(item);   
}

TreeItem* TreeItem::getChild(QString str){
dbg;  
    int i=0;
    while (i<childItems.size() && childItems.at(i)->values.at(0) != str)
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
    return tables->contains(values.at(0));
}
                                                     
void TreeItem::removeChild( QString name ){
dbg;
    int i=0;
    while (i < childItems.size() && childItems.at(i)->values.at(0) != name)
        i++;
    if (childItems.at(i)->values.at(0) == name){
        delete childItems.at(i);
        childItems.removeAt(i);
    }
}

void TreeItem::setEnds( QString b, QString e){
dbg;
    values.replace(3, b); // beginsWith
    values.replace(4, e); // endsWith    
}

QVariant TreeItem::data( int i ) const { 
dbg;
    if (i<=values.size())
        return values.at(i);
     return QVariant();   
}
