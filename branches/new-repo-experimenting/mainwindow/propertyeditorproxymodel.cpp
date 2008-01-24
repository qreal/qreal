#include <QDebug>

#include "propertyeditorproxymodel.h"
#include "realreporoles.h"

PropertyEditorModel::PropertyEditorModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int PropertyEditorModel::rowCount(const QModelIndex&) const
{
	return roleNames.size();
}

int PropertyEditorModel::columnCount(const QModelIndex&) const
{
	return 1;
}

Qt::ItemFlags PropertyEditorModel::flags (const QModelIndex &/*index*/) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant PropertyEditorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
//	if ( ! targetModel )
//		return QVariant();

	if ( role == Qt::DisplayRole && orientation == Qt::Vertical )
		return roleNames.at(section);
	else
		return QVariant();
}

QVariant PropertyEditorModel::data(const QModelIndex &index, int role) const
{
//	if ( ! targetModel )
//		return QVariant();

	if ( role == Qt::DisplayRole && index.column() == 0 ) {
		return targetObject.data(info.roleByIndex(index.row()));
	} else
		return QVariant();
}

bool PropertyEditorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if ( ! targetModel )
		return false;

	if ( ( role == Qt::DisplayRole || role == Qt::EditRole ) && index.column() == 0 )
		return targetModel->setData(targetObject, value, info.roleByIndex(index.row()));
	else
		return false;
}

void PropertyEditorModel::rereadData()
{
	reset();
}

void PropertyEditorModel::setSourceModel(QAbstractItemModel *sourceModel)
{
	targetModel = sourceModel;

	roleNames.clear();

	if ( targetModel )
		connect ( targetModel, SIGNAL( dataChanged (const QModelIndex &, const QModelIndex&) ),
				this, SLOT( rereadData() ) );

	reset();
}

void PropertyEditorModel::setIndex(const QModelIndex &sourceIndex)
{
	if ( sourceIndex.model() != targetModel )
		return;

	targetObject = sourceIndex;
	type = targetObject.data(Unreal::TypeRole).toInt();

	roleNames = info.getColumnNames(type);

	reset();
}


