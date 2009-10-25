/** @file propertyeditorproxymodel.cpp
 *	@brief Модель редактора свойств
 * */
#include <QDebug>

#include "propertyeditorproxymodel.h"
#include "../model/model.h"

using namespace qReal;

PropertyEditorModel::PropertyEditorModel(qReal::EditorManager const &editorManager,
										 QObject *parent)
	: QAbstractTableModel(parent), mPseudoAttributesCount(0), mEditorManager(editorManager)
{
}

int PropertyEditorModel::rowCount(const QModelIndex&) const
{
	return mFieldNames.size();
}

int PropertyEditorModel::columnCount(const QModelIndex&) const
{
	return 2;
}

Qt::ItemFlags PropertyEditorModel::flags(QModelIndex const &index) const
{
	// Property names
	if (index.column() == 0)
		return Qt::ItemIsEnabled;

	// Object id
	if (index.row() < mPseudoAttributesCount)
		return Qt::NoItemFlags;

	// Other properties
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant PropertyEditorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
		return QString(section ? "value" : "name");
	else
		return QVariant();
}

QVariant PropertyEditorModel::data(QModelIndex const &index, int role) const
{
	if (!targetModel)
		return QVariant();

	if (!targetObject.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	if (index.column() == 0) {
		return mFieldNames.at(index.row());
	} else if (index.column() == 1) {
		if (index.row() >= mPseudoAttributesCount)
			return targetObject.data(roleByIndex(index.row()));
		else if (index.row() == 0) {
			Id id = targetObject.data(roles::idRole).value<Id>();
			return QVariant(id.editor() + "/" + id.diagram() + "/" + id.element());
		}
		else
			return targetObject.data(roles::idRole).value<Id>().id();
		return QVariant();
	} else
		return QVariant();
}

bool PropertyEditorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	bool ret = false;

	if (!targetModel)
		return false;

	if (!targetObject.isValid())
		return false;

	if ((role == Qt::DisplayRole || role == Qt::EditRole ) && index.column() == 1)
		if (index.row() != 0) {
			 model::Model *im = const_cast<model::Model *>(static_cast<model::Model const *>(targetModel));
			 ret = im->setData(targetObject, value, roleByIndex(index.row()));
		}
		else
			ret = true;
	else
		ret = false;
	if (ret)
		dataChanged(index, index);
	return ret;
}

int PropertyEditorModel::roleByIndex(int const index) const
{
	return index - mPseudoAttributesCount + roles::customPropertiesBeginRole;
}

void PropertyEditorModel::rereadData()
{
	reset();
}

void PropertyEditorModel::setSourceModel(QAbstractItemModel *sourceModel)
{
	targetModel = sourceModel;

	mFieldNames.clear();

	if (targetModel)
		connect(targetModel, SIGNAL(dataChanged(QModelIndex const &, QModelIndex const &)),
				this, SLOT(rereadData()));

	reset();
}

void PropertyEditorModel::setIndex(const QModelIndex &sourceIndex)
{
	// Special case - no objects selected
	if (sourceIndex == QModelIndex())
	{
		targetObject = sourceIndex;
		mFieldNames.clear();
		reset();
		return;
	}

	if (sourceIndex.model() != targetModel)
		return;

	targetObject = sourceIndex;

	Id id = targetObject.data(roles::idRole).value<Id>();
	mFieldNames = mEditorManager.getPropertyNames(id.type());

	mFieldNames.push_front("repo_id");
	mFieldNames.push_front("metatype");
	mPseudoAttributesCount = 2;

	reset();
}
