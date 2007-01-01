//=====================================================================
// Project:      altREAL
// File Name:    treemodel.cpp
// Description:  Qt Repository-based Model
//
// Date:         21-November-06
// Revision:      
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#include <QtGui>
#include "treemodel.h"
#include "entity_types.h"

TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent) {
  
  // 'root' contains 'ent1', 'ent2' + link between them
  
  R_ObjId rootId = R_ObjId::makeId(666, typeDiagram_Class); 
  int curId     = rootId.objId;
  root = new R_Entity(rootId, rootId, repa, 0);
  root->setValue("name", "r00t");
  root->setValue("property", "ochen' kr00t");

  R_ObjId newId1 = R_ObjId::makeId(14, typeComment);
  ent1 = new R_Entity(newId1, rootId, repa, 0);
  ent1->setValue("name", "medved");
  ent1->setValue("property", "preved! \\_0_/");
  
  R_ObjId newId2 = R_ObjId::makeId(88, typeComment);
  ent2 = new R_Entity(newId2, rootId, repa, 0); 
  ent2->setValue("name", "maped");
  ent2->setValue("property", "suzuki GSF-S600");

  R_ObjId linkId = R_ObjId::makeId(333);
  link = new R_Entity(linkId, rootId, repa, 0);
  link->setValue("name", "link");
  link->setValue("property", "bred");

  root->addChild(ent1);
  root->addChild(ent2);
  
  ent1->addLink(ent2);
  
  printf("children = %d\n", root->getChildsCount());
  
}

void TreeModel::parseFile(const QString &fileName){
}

TreeModel::~TreeModel(){
    
  //delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const{

  return 2; 

//  if (parent.isValid())
//    return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
//  else
//    return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const{

 if (!index.isValid())
   return QVariant();
 if (role != Qt::DisplayRole)
   return QVariant();
 int col = index.column();

#ifdef DEBUG
 printf("data(): row: %d   , col: %d\n", index.row(), index.column());
#endif

 R_Entity *item = static_cast<R_Entity*>(index.internalPointer());
 if(col==0)
    return item->getValue("name").c_str();
 else 
    return item->getValue("property").c_str();
    
// return item->data(index.column());
}

bool TreeModel::setData(const QModelIndex &index,
                               const QVariant &value, int role) {
                              
  if (index.isValid() && role == Qt::EditRole) {
     R_Entity *item = static_cast<R_Entity*>(index.internalPointer());
     std::string val;
     if (index.column() == 0) 
        val = "name";
     else if (index.column() == 1)
        val = "property";
     item->setValue(val, value.toString().toStdString());
     emit dataChanged(index, index);
     return true;
  }
  return false;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const{

 if (!index.isValid())
   return Qt::ItemIsEnabled;
 return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const {
                               
  std::string val;
  if (section == 0) 
     val = "name";
  else if (section == 1)
     val = "property";

  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return val.c_str();
    
 return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const{

#ifdef DEBUG
  //printf("index(): row: %d   , col: %d\n", row, column);
#endif

  R_Entity *parentItem;
  if (!parent.isValid()){
//    printf("index():  not valid\n");
   // return QModelIndex();
    parentItem = root;
  } 
  else
    parentItem = static_cast<R_Entity*>(parent.internalPointer());

  std::vector<int> a;
  a.push_back(typeComment);
  if(parentItem->getValue("name") == "r00t")
    if (row == 0)
      return createIndex(row, column, (void*) ent1);
    else if (row == 1)
      return createIndex(row, column, (void*) ent2);
    else if (row == 2)
      return createIndex(row, column, (void*) link);
 
// R_Entity *childItem = parentItem->child(row);
// if (childItem)
//   return createIndex(row, column, childItem);
// else
   return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const{

 if (!index.isValid())
   return QModelIndex();
 R_Entity *childItem = static_cast<R_Entity*>(index.internalPointer());
 R_ObjId parentId = childItem->getOwner();
 if (parentId.equals(root->getId()))
   return QModelIndex();
 //return createIndex(parentItem->row(), 0, parentItem);
 return createIndex(0, 0, (void*) &root); //quick hack
}

int TreeModel::rowCount(const QModelIndex &parent) const{

#ifdef DEBUG
 printf("rowCount():  row: %d   , col: %d\n", parent.row(), parent.column());
#endif

 R_Entity *parentItem;
 if (!parent.isValid()){
   //printf("not valid: using r00t\n");
   parentItem = root;
 }  
 else
   parentItem = static_cast<R_Entity*>(parent.internalPointer());
 //int rows =  parentItem->getChildsCount();
 if (parentItem->getValue("name") == "r00t"){  // quick dirty hack
   //printf("returning 3\n");
   return 3;                           // should be rows + all children's links count
 }   
 else {                                   // too lazy for that right now 
   // printf("returning 0\n");
    return 0;
 }   
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent){
	
	beginInsertRows(QModelIndex(), position, position + rows - 1);
//  TreeItem *item = static_cast<TreeItem*>(parent.internalPointer());
//  QList<QVariant> tmp;
//	tmp << "tmp1" << "tmp2";

	//for (int row = 0; row < rows; ++row )
//		item->childItems.insert(position + row, new TreeItem(tmp, item));
	
	endInsertRows();
	return true;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent){
 
 beginRemoveRows(QModelIndex(), position, position + rows - 1);
// TreeItem *item = static_cast<TreeItem*>(parent.internalPointer());
// for(int row=0; row < rows; ++row)
//   item->childItems.removeAt(row);
 endRemoveRows();
 return true;
}


