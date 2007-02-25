//=====================================================================
// Project:      unREAL 
// File Name:    propertyeditormodel.cpp 
// Description:  Model for Property Editor support
//
// Created:      24-Feb-07
// Revision:     25-Feb-07 
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#define _LONG_DEBUG
#include "propertyeditormodel.h"
#include "dbg.h"


PropertyEditorModel::PropertyEditorModel(QSqlDatabase &_db, DiagramExplorerModel *_dem, QObject *parent) : QAbstractTableModel(parent) {
dbg;
    dem = _dem;
    db = _db;
    diagram = "";
    name = "";
    rootd = static_cast<TreeItem*>(dem->index(0,0,QModelIndex()).internalPointer()); 
//    rootd = rootd->parent();
}

void PropertyEditorModel::rescan(const QModelIndex &index ){
dbg;
    
    current = index;
    TreeItem *ti = static_cast<TreeItem*>(index.internalPointer());

    diagram = ti->getDiagramName();
    type    = ti->getType();
    name    = ti->getName();

qDebug() << name << type << diagram;

    QSqlQuery q;  
    QString tmp;
    if( type == "diagram"){  // get diagram info
        tmp = "select * from %1 where name='%2'";
        tmp = tmp.arg(type).arg(name);
    }
    else { // get element info
        tmp = "select * from %1 where name='%2' and diagram='%3'";
        tmp = tmp.arg(type).arg(name).arg(diagram);
    }    
    q = db.exec(tmp);
    QSqlRecord r = q.record();

    while(q.next()){ 
        fields.clear(); 
        vals.clear();
        for( int i = 0; i < r.count(); i++ ){
            fields << r.fieldName(i);
            vals   << q.value(i).toString();
        }
    }
}

void PropertyEditorModel::adjustData(const QModelIndex &index ){
dbg;    
    rescan( index );
}


void PropertyEditorModel::updateName( QStringList list ){
dbg;
    
    vals.replace(fields.indexOf("name"), list.at(1));
    QModelIndex ind = createIndex(fields.indexOf("name"), 1, index(fields.indexOf("name"),1,QModelIndex()).internalPointer());
    emit dataChanged(ind, ind);
}


PropertyEditorModel::~PropertyEditorModel(){
dbg;    
}

bool PropertyEditorModel::setData(const QModelIndex& index, const QVariant &value, int role){
dbg;
    if (index.isValid() && role == Qt::EditRole) {
        

        if (rootd->getType() == "diagram"){
            QMessageBox::critical(0, tr("error"), tr("sorry, you should donate to use this feature"));
            return false;
        }
        
        if( elementExists(value.toString(), type, diagram) <= 0)
            return false;
        
        QString field  = fields.at(index.row());
        QString oldval = vals.at(index.row());
        
        QString tmp = "update %1 set %2='%3' where %2='%4' and diagram='%5'";
        QString tmp2 = tmp.arg(type).arg(field).arg(value.toString()).arg(oldval).arg(diagram);
        db.exec(tmp2);

        if( field == "name" ){
            tmp2 = tmp.arg(diagram).arg(field).arg(value.toString()).arg(oldval);
            db.exec(tmp2);
            name = value.toString();
            QStringList list;
            list << oldval << value.toString() << diagram << type;
            emit nameChanged( list );
        }

        if( field == "diagram"){
        
        }
        
        vals.replace(index.row(), value.toString());
       
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

int PropertyEditorModel::columnCount(const QModelIndex &) const{
dbg;
    return 2;
}

QVariant PropertyEditorModel::data(const QModelIndex &index, int role) const{
//dbg;
    if (!index.isValid())
        return QVariant();
    if (role != Qt::DisplayRole)
        return QVariant();

    int col = index.column();
    int row = index.row();
    QString res;
   
    if (col == 0)
        res = fields.at(row);
    else if (col == 1)
        res = vals.at(row);
    return res;
}

Qt::ItemFlags PropertyEditorModel::flags(const QModelIndex &index) const{
dbg;
    Qt::ItemFlags f = Qt::ItemIsEnabled;
    if (!index.isValid())
        return f;
    if (index.column() == 1)
        f |= Qt::ItemIsEditable;
    return f | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant PropertyEditorModel::headerData(int , Qt::Orientation , int ) const {
dbg;                               
    return "";
}

/*
QModelIndex PropertyEditorModel::index(int row, int column, const QModelIndex &parent) const{
dbg;            
    return createIndex(row, column, QModelIndex());
}

QModelIndex PropertyEditorModel::parent(const QModelIndex &index) const{
dbg;
    return QModelIndex();
}
*/

int PropertyEditorModel::rowCount(const QModelIndex &) const{
dbg;
    return vals.size();
}

int PropertyEditorModel::elementExists( QString name, QString , QString diagram){
dbg;
    TreeItem* par = rootd->getChild(diagram);
    qDebug() << diagram;
    qDebug() << par;
    qDebug() << par->getName();
    if (!par){
        QMessageBox::critical(0, QObject::tr("error"), QObject::tr("requested diagram not found.\nyou should create diagram first"));
        return -1;
    }   
    TreeItem *child = par->getChild(name);
    if(child){
        QMessageBox::critical(0, QObject::tr("error"), QObject::tr("element with such name already exists.\nchoose another name plz"));
        return 0;
    }
    return 1;
}

