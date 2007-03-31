//=====================================================================
// Project:      unREAL
// File Name:    diagramexplorermodel.cpp
// Description:  Proxy model for Diagram Explorer
//
// Created:      16-Jan-07
// Revision:     21-Mar-07
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

//#define _LONG_DEBUG
#include "diagramexplorermodel.h"
#include "dbg.h"


DiagramExplorerModel:: DiagramExplorerModel(QSqlDatabase &_db, QObject *parent) : QAbstractItemModel(parent) {
dbg;

    db = _db;
    QStringList l;
    l << "diagram" << "diagram" << "";
    elements = new QMap<int, TreeItem*>;
    diagrams = new QMap<QString, QString>;
    rootItem = new TreeItem(l, diagrams, 0, db);   
    rootItem->setID(-1);
    diagramsList.clear();
    maxID = 0;

    //FIXME: temporary kludge

    QMap<int, QString> a1;
    a1.insert(0,"uuid");
    a1.insert(1,"name");
    a1.insert(2,"type");
    a1.insert(3,"diagram");
    a1.insert(4,"x");
    a1.insert(5,"y");
    a1.insert(6,"status");
    a1.insert(7,"description");
    a1.insert(8,"priority");
    a1.insert(9,"source");
    QMap<int, QString> a2;
    a2.insert(0,"uuid");
    a2.insert(1,"name");
    a2.insert(2,"type");
    a2.insert(3,"diagram");
    a2.insert(4,"x");
    a2.insert(5,"y");
    a2.insert(6,"status");
    a2.insert(7,"beginsWith");
    a2.insert(8,"endsWith");

    props.insert("eP2N", a2);
    props.insert("non-eP2N", a1);

    rescan();  
}  

int DiagramExplorerModel::getID(){
dbg;
    return maxID;
}

int DiagramExplorerModel::getNextID(){
dbg;
    return (++maxID);
}

void DiagramExplorerModel::rescan(){
dbg;

    diagramsList.clear();
    elements->clear();

    TreeItem *table, *value;
    QString tmp;
    QString tmpID;

    QSqlQuery q,q1,q2,q3;  
    QStringList l;

    tmp = "select MAX(uuid) from diagram";
    q = db.exec(tmp);
    q.next();
    maxID = q.record().value(0).toInt();
    tmp = "select * from diagram where type='diagrams'";
    diagrams->insert("diagram", tmp);
  
    q1 = db.exec(tmp);
    while(q1.next()){           // fetching diagram names
        QString tableName = q1.value(q1.record().indexOf("name")).toString();    
        QString statusName = q1.value(q1.record().indexOf("status")).toString();    
        int id = q1.value(q1.record().indexOf("uuid")).toInt();
        if (maxID < id)
            maxID = id;
        l.clear(); 
        l << QString::number(id) << tableName << "diagram" << "diagram" << "0" << "0" << statusName << "0" << "0";
        table = new TreeItem(l, diagrams, rootItem, db);                 
        diagramsList << tableName;
        rootItem->addChild(table);
        if (elements->contains(id))
            QMessageBox::warning(0, tr("mmmm..."), tr("something weird with diagrams IDs"));
        elements->insert(id, table);

        tmp = "select * from " + tableName;
        diagrams->insert(tableName, tmp);
        q3 = db.exec(tmp);
        while(q3.next()){
            QString valueName = q3.value(q3.record().indexOf("name")).toString();
            QString typeName  = q3.value(q3.record().indexOf("type")).toString();
            QString x  = q3.value(q3.record().indexOf("x")).toString();
            QString y  = q3.value(q3.record().indexOf("y")).toString();
            int curID = q3.value(q3.record().indexOf("uuid")).toInt();
	        QString status = q3.value(q3.record().indexOf("status")).toString();
            //qDebug() << valueName << curID;
            if ( curID > maxID )
                maxID = curID;
            l.clear();
            l << QString::number(curID) << valueName << typeName << tableName << x << y << status;
            qDebug() << l;
            q2 = db.exec("select * from " + typeName + " where name='" + valueName + "'");
            if (!q2.next()){
                qDebug() << "there's no such element in the db, sorry...";
                return;
            }    
            for (int i=2; i<q2.record().count()-1; i++){
                l << q2.value(i).toString();
            }    
            value = new TreeItem(l, diagrams, table, db);
            value->setID(curID);
            table->addChild(value);
            if (elements->contains(curID))
                QMessageBox::warning(0, tr("mmmm..."), tr("something weird with elements IDs"));
            elements->insert(curID, value);
        }    
    }

}

DiagramExplorerModel::~ DiagramExplorerModel(){
dbg;    
  delete rootItem;
}

bool  DiagramExplorerModel::setData(const QModelIndex& index, const QVariant &value, int role){
dbg;

    int col = index.column();

    QString fld;

    if (index.isValid() && role == Qt::EditRole) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

      //  qDebug() << "ID: " << item->getID();

        if (item->getType() == "eP2N")
            fld = props.value("eP2N").value(col);
        else
            fld = props.value("non-eP2N").value(col);

        if (item->getType() == "diagram"){
  
            QMessageBox::critical(0, tr("error"), tr("sorry, you should donate to use this feature.\n"));

        }
        else{

//    qDebug() << col << fld;

            if( col == 1 ){
                if ( elementExists( value.toString(), item->getType(), item->getDiagramName()) <= 0)
                    return false;
    

                QStringList list;
                list << item->getName() << value.toString() << item->getDiagramName() << item->getType();
                emit nameAboutToBeChanged(list);
            }    
    
            db.exec("update "+item->getDiagramName()+" set "+fld+"='"+value.toString()+"'"
                                                                " where name='" + item->getName() + "'");
            qDebug() << "update "+item->getDiagramName()+" set "+fld+"='"+value.toString()+"'"
                                                                " where name='" + item->getName() + "'";
            db.exec("update " + item->getType() + " set "+fld+"='" + value.toString() + "'"
                                                                " where name='" + item->getName() + "'");
            qDebug() << "update " + item->getType() + " set "+fld+"='" + value.toString() + "'"
                                                                " where name='" + item->getName() + "'";
            item->setData(col, value.toString());
        }

        emit dataChanged(index, index);
        return true;
    }
  return false;
}

void DiagramExplorerModel::nameChanged( QStringList list ){
dbg;
    QString oldname = list.at(0);
    QString diagram = list.at(2);
    QString newname = list.at(1);
    TreeItem *it = rootItem->getChild(diagram)->getChild(oldname);
    it->setName(newname);
    
    QModelIndex index = createIndex(it->row(), 0, (void*) it);
    emit dataChanged(index, index);
}


int DiagramExplorerModel::columnCount(const QModelIndex &parent) const{
dbg;
  if (parent.isValid())
    return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
  else // ---BEGIN TMP FIX---
    return rootItem->columnCount();
}

QVariant DiagramExplorerModel::data(const QModelIndex &index, int role) const{
dbg;
 if (!index.isValid())
   return QVariant();
 if (role != Qt::DisplayRole)
   return QVariant();
 TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    //------BEGIN TMP FIX------
    if ( index.column() == 0 )
	return item->getID();
    else
	return item->data(index.column());
}

Qt::ItemFlags DiagramExplorerModel::flags(const QModelIndex &index) const{
dbg;
  Qt::ItemFlags f = Qt::ItemIsEnabled;
  if (!index.isValid())
    return f;
  return f | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant DiagramExplorerModel::headerData(int , Qt::Orientation , int ) const {
dbg;                               
 return "";
}

QModelIndex DiagramExplorerModel::index(int row, int column, const QModelIndex &parent)
            const{
dbg;            
    TreeItem *parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->getChild(row);

    if (childItem){
        return createIndex(row, column, childItem);
    }  
    else
         return QModelIndex();
    }

QModelIndex DiagramExplorerModel::parent(const QModelIndex &index) const{
dbg;
 if (!index.isValid())
   return QModelIndex();
 TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
 if (childItem == rootItem)
    return QModelIndex();
 TreeItem *parentItem = childItem->parent();
 if (parentItem == rootItem)
   return QModelIndex();
 return createIndex(parentItem->row(), 0, parentItem);
}

int DiagramExplorerModel::rowCount(const QModelIndex &parent) const{
dbg;
  TreeItem *parentItem;
  if (!parent.isValid()){
    parentItem = rootItem;
  }  
  else{
    parentItem = static_cast<TreeItem*>(parent.internalPointer());

  }  
  return parentItem->rowCount();
}

void DiagramExplorerModel::createDiagramScriptsExec(QStringList vals){
dbg;
    QString name   = vals.at(0);
    QString status = vals.at(1);
    QString tmp = "insert into diagram (uuid, name, type, status) values (%1, '%2', 'diagrams', '%3')";
    tmp = tmp.arg(getID()).arg(name).arg(status);
    db.exec(tmp);
    
    tmp = "create table " + name + " (uuid integer, name varchar(20), type varchar(20),"
                                                " status varchar(20), x integer, y integer)";
    db.exec(tmp);
}

void DiagramExplorerModel::removeDiagramScriptsExec(const QString &name){
dbg;
    QString tmp;
    QStringList l;

    TreeItem *d = rootItem->getChild(name);
    int cnt = d->childCount();
    
    for (int i=0; i<cnt; i++){  // mutilate children! ARRRRRRRRRRRRRRRGGGGGGGHHH!!
        l.clear();
        l << d->getChild(0)->getName() << name;
        remove(true, l);
    }    
    
    tmp = "drop table %1";
    db.exec(tmp.arg(name));
    tmp = "delete from diagram where name='%1'";
    db.exec(tmp.arg(name));
}

void DiagramExplorerModel::createElementScriptsExec(QStringList values, QString fields){
dbg;
    QString name    = values.at(0);
    QString type    = values.at(1);
    QString diagram = values.at(2);
    QString x       = values.at(3);
    QString y       = values.at(4);
    QString status  = values.at(5);
    
    QString tmp = "insert into %1 (uuid, name, type, status, x, y) values (%2, '%3', '%4', '%5', %6, %7)";
    tmp = tmp.arg(diagram).arg(getID()).arg(name).arg(type).arg(status).arg(x).arg(y);
    db.exec(tmp);

    if (type != "eP2N"){
        QString desc    = values.at(6);
        QString prior   = values.at(7);
        QString source  = values.at(8);
    
        tmp = "insert into %1 (%2) values (%3, '%4', '%5', %6, '%7', '%8', '%9')";
        tmp = tmp.arg(type).arg(fields).arg(getID()).arg(name).arg(desc).arg(prior).arg(source).arg(status).arg(diagram);
        db.exec(tmp);
    }
    else{
        QString from = values.at(6);
        QString to   = values.at(7);
        
        tmp = "insert into %1 (%2) values (%3, '%4', '%5', '%6', '%7', '%8')";
        tmp = tmp.arg(type).arg(fields).arg(getID()).arg(name).arg(from).arg(to).arg(status).arg(diagram);
        db.exec(tmp);
    }
}

void DiagramExplorerModel::removeElementScriptsExec( QStringList vals ){
dbg;   
    QString name    = vals.at(0);
    QString diagram = vals.at(1);
    QString type = rootItem->getChild(diagram)->getChild(name)->getType();
    QString tmp = "delete from %1 where name='%2'";
    db.exec(tmp.arg(diagram).arg(name));
    db.exec(tmp.arg(type).arg(name));
}    

QModelIndex DiagramExplorerModel::getDiagramIndex( QString name ){
dbg;
    if (name != "")
        return  createIndex(rootItem->getChild(name)->row(),0,(void*)rootItem->getChild(name));
    else 
        return QModelIndex();
}

QModelIndex DiagramExplorerModel::getIndex(QString id){
dbg;
    TreeItem* item = elements->value(id.toInt());//rootItem->getChild(diagram)->getChild(name);
    return createIndex(item->row(),0,item);
}


QModelIndex DiagramExplorerModel::getBeginning( QModelIndex& index ){
dbg;
    TreeItem* it = static_cast<TreeItem*>(index.internalPointer());
    return getIndex(it->getBeginning());
}
QModelIndex DiagramExplorerModel::getEnding( QModelIndex& index ){
dbg;
    TreeItem* it = static_cast<TreeItem*>(index.internalPointer());
    return getIndex(it->getEnding());
}

void DiagramExplorerModel::insert(bool isElement, QString fields, QStringList values){
dbg;
   
    QModelIndex index = QModelIndex();
    TreeItem* par; 

    getNextID();
    values << QString::number(getID());

    if (!values.at(2).isEmpty()){
        if (isElement)
            par = rootItem->getChild(values.at(2));
        else
            par = rootItem;
        index = createIndex( par->row(), 0, (void*) par );
    }        
        
    if (!insertRows(rowCount(index), 1, fields, values, index))
        qDebug() << "cannot create new row"; 

    if (isElement)
        emit elemAdded(values);    
    else{
        diagramsList << values.at(0);
    }    
}

bool  DiagramExplorerModel::insertRows(int position, int rows, QString fields, QStringList vals, const QModelIndex &parent){
dbg;
    
    if (!vals.at(2).isEmpty()) 
        beginInsertRows(parent, position, position + rows - 1);
    
    QString name;
    QString type;
    QString diagram;
    QString status;
    QString x;
    QString y;

    QStringList l;

    l << QString::number(getID());

    if ( fields == "" ){ // creating diagram in the database
        createDiagramScriptsExec(vals);
        name    = vals.at(0);
        status  = vals.at(1);
        type    = "diagram";
        diagram = "diagram";
        l << name << type << diagram;  
    }    
    else{ // creating element in the database
        createElementScriptsExec(vals, fields);
        name    = vals.at(0);
        type    = vals.at(1);
        diagram = vals.at(2);
        x       = vals.at(3);
        y       = vals.at(4);
        status  = vals.at(5);
        l << name << type << diagram << status << x << y;
        for (int i=3; i<vals.size(); i++)
            l << vals.at(i);
    }    

    if( fields == ""){
        QString tmp = "select * from " + name;
        diagrams->insert(name, tmp);
    }
    
    if(!diagram.isEmpty()){

        TreeItem *par;
        if(parent.isValid())
            par = static_cast<TreeItem*>(parent.internalPointer());
        else 
            par = 0;     
    
    
        TreeItem *child = new TreeItem(l, diagrams, par, db);
        child->setID(getID());
  
   
        if (type == "eP2N"){
            QString beginning = vals.at(6);
            QString ending    = vals.at(7);
            child->setEnds(beginning, ending);
        }
    
        if (par)
            par->addChild(child);

        if( elements->contains(getID()))
            QMessageBox::warning(0, tr("mmmm..."), tr("something weird with IDs..."));
        elements->insert(getID(), child);
    }
    if(!vals.at(2).isEmpty())
	    endInsertRows();

	return true;
}

void DiagramExplorerModel::remove(bool isElement, QStringList values){
dbg;
    QModelIndex index;
    TreeItem* par; 
    if (isElement){
        par = rootItem->getChild(values.at(1));
        values << par->getChild(values.at(0))->getType();
    } else
        par = rootItem;
    
    index = createIndex( par->row(), 0, (void*) par );
       
    if (!removeRows(par->getChild(values.at(0))->row(), 1, isElement, values, index))
        qDebug() << "cannot remove row"; 

    if (isElement)
        emit elemRemoved(values);    
    else{
        diagramsList.removeAt(diagramsList.indexOf(values.at(0)));
    }
    
}


bool  DiagramExplorerModel::removeRows(int position, int rows, bool isElement, QStringList vals, const QModelIndex &parent){
dbg; 
    beginRemoveRows(parent, position, position + rows - 1);
    if ( !isElement ) // removing diagram from the database
        removeDiagramScriptsExec(vals.at(0));
    else // removing element from the database
        removeElementScriptsExec(vals);
    
    TreeItem *par;
    if(parent.isValid())
        par = static_cast<TreeItem*>(parent.internalPointer());
    else 
        return false;
    
    QString name = vals.at(0);
    par->removeChild(name);
    
    if( !isElement )
        diagrams->remove(name);
    
    endRemoveRows();
    return true;
}

int DiagramExplorerModel::elementExists( QString name, QString , QString diagram){
dbg;
TreeItem* par = rootItem->getChild(diagram);
    if (!par){
        QMessageBox::critical(0, QObject::tr("dem error"), QObject::tr("requested diagram not found.\nyou should create diagram first"));
        return -1;
    }   
    TreeItem *child = par->getChild(name);
    if(child){
        QMessageBox::critical(0, QObject::tr("error"), QObject::tr("element with such name already exists.\nchoose another name plz"));
        return 0;
    }
    return 1;
}

int DiagramExplorerModel::diagramExists( QString name ){
dbg;
    TreeItem* par = rootItem->getChild(name);
    if (par){
        QMessageBox::critical(0, QObject::tr("error"), QObject::tr("such diagram already exists.\ntry another name"));
        return 1;
    }   
    return 0;
}
