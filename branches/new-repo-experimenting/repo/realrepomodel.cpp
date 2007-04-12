#include <QtGui>
#include <QtXml>

#include "realrepoitem.h"
#include "realrepomodel.h"

RealRepoModel::RealRepoModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new RealRepoItem(RealRepoItem::Root,0,0,0);
}

RealRepoModel::~RealRepoModel()
{
    delete rootItem;
}

static QColor rndColor()
{
    using namespace Qt;
    QList<QColor> list;
    list << white << black << red << darkRed << green << darkGreen <<
	    blue << darkBlue << cyan << darkCyan << magenta << darkMagenta <<
	    yellow << darkYellow << lightGray;
    
    return list[random()%list.size()];
}

QVariant RealRepoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    RealRepoItem *item = static_cast<RealRepoItem*>(index.internalPointer());

    switch ( role ) {
	case Qt::DisplayRole:
	case Qt::EditRole:		return item->name();
    }

    return QVariant();
}

bool RealRepoModel::setData(const QModelIndex &index, const QVariant &value, int role) 
{
    RealRepoItem *item = static_cast<RealRepoItem*>(index.internalPointer());

    switch ( role ) {
	case Qt::DisplayRole:
	case Qt::EditRole:		item->setName(value.toString());	break;
	default:			return false;
    }

    foreach(RealRepoItem *item, listByUuid[item->id()]) {
	item->updateData();
	QModelIndex index = getIndexByItem(item);
	emit dataChanged(index,index);
    }

    return true;
}

Qt::ItemFlags RealRepoModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    
    if ( static_cast<RealRepoItem*>(index.internalPointer())->id() > 100 )
	flags |= Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

    return flags;
}

Qt::DropActions RealRepoModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QVariant RealRepoModel::headerData(int section, Qt::Orientation orientation,
                              int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Name");
            default:
                return QVariant();
        }
    }

    return QVariant();
}

bool RealRepoModel::insertRows (int row, int count, const QModelIndex &parent)
{
    qDebug() << __PRETTY_FUNCTION__;
    return false;
}

bool RealRepoModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
		int row, int column, const QModelIndex &parent)
{
//	if (action == Qt::IgnoreAction)
//		return true;

	qDebug() << row << column << parent << static_cast<RealRepoItem*>(parent.internalPointer())->name();
	
	return true;
}
			    

QModelIndex RealRepoModel::index(int row, int column, const QModelIndex &parent)
            const
{
    RealRepoItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<RealRepoItem*>(parent.internalPointer());

    RealRepoItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex RealRepoModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    RealRepoItem *childItem = static_cast<RealRepoItem*>(child.internalPointer());
    RealRepoItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int RealRepoModel::rowCount(const QModelIndex &parent) const
{
    RealRepoItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<RealRepoItem*>(parent.internalPointer());

    return parentItem->rowCount();
}

int RealRepoModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

QModelIndex RealRepoModel::getIndexByItem(const RealRepoItem *item)
{
    QList <int> rowCoords;

    for ( RealRepoItem *curItem = const_cast<RealRepoItem *>(item);
	    curItem != rootItem; curItem = curItem->parent() ) {
	rowCoords.append(curItem->row());
    }

    QModelIndex result;

    for ( int i = rowCoords.size() - 1; i >= 0; i-- ) {
        result = index(rowCoords[i],0,result);
    }

    return result;
}

