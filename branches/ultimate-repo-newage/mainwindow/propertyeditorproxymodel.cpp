/** @file propertyeditorproxymodel.cpp
 *	@brief Модель редактора свойств
 * */
#include <QDebug>

#include "propertyeditorproxymodel.h"
#include "realreporoles.h"

PropertyEditorModel::PropertyEditorModel(QObject *parent)
	: QAbstractTableModel(parent), type(0), mPseudoAttributesCount(0)
{
}

int PropertyEditorModel::rowCount(const QModelIndex&) const
{
	return roleNames.size();
}

int PropertyEditorModel::columnCount(const QModelIndex&) const
{
	return 2;
}

Qt::ItemFlags PropertyEditorModel::flags (const QModelIndex &index) const
{
	// Property names
	if (index.column() == 0)
		return Qt::ItemIsEnabled;

	// Object id
	if (index.row() == 0)
		return Qt::NoItemFlags;

	// Other properties
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant PropertyEditorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
//	if ( ! targetModel )
//		return QVariant();

	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
		return QString(section ? "value" : "name");
	else
		return QVariant();
}

QVariant PropertyEditorModel::data(const QModelIndex &index, int role) const
{
//	if ( ! targetModel )
//		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	if (index.column() == 0) {
		return roleNames.at(index.row());
	} else if (index.column() == 1) {
		if (index.row() != 0)
			return targetObject.data(info.roleByIndex(index.row() - mPseudoAttributesCount));
		else
			return QVariant(info.objectName(type));
	} else
		return QVariant();
}

bool PropertyEditorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	bool ret;

	if ( ! targetModel )
		return false;

	if ( ( role == Qt::DisplayRole || role == Qt::EditRole ) && index.column() == 1 )
		if (index.row() != 0)
			ret = targetModel->setData(targetObject, value, info.roleByIndex(index.row() - mPseudoAttributesCount));
		else
			ret = true;
	else
		ret = false;
	if (ret)
		dataChanged(index, index);
	return ret;
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

	roleNames.push_front("metatype");
	mPseudoAttributesCount = 1;

	reset();
}
