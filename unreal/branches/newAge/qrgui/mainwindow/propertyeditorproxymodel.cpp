/** @file propertyeditorproxymodel.cpp
 *	@brief Модель редактора свойств
 * */
#include <QDebug>

#include "propertyeditorproxymodel.h"

using namespace qReal;

PropertyEditorModel::PropertyEditorModel(qReal::EditorManager const &editorManager,
										 QObject *parent)
	: QAbstractTableModel(parent), mPseudoAttributesCount(0), mEditablePseudoAttributesCount(0),
	mEditorManager(editorManager)
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
	if (index.row() < mPseudoAttributesCount - mEditablePseudoAttributesCount)
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
		} else if (index.row() == 1)
			return targetObject.data(roles::idRole).value<Id>().id();
		else if (index.row() == 2) {
			return targetObject.data(Qt::DisplayRole);
		}
		return QVariant();
	} else
		return QVariant();
}

QStringList PropertyEditorModel::getEnumValues(const QModelIndex &index) const
{
//	if (index.row() <= 2) // metatype, id and name are definitely not enums
//		return QStringList();
//	model::Model *im = const_cast<model::Model *>(static_cast<model::Model const *>(targetModel));
//	if (im){
//		return im->getEnumValues(targetObject, roleByIndex(index.row()));
//	}
	return QStringList();
}

QString PropertyEditorModel::getTypeName(const QModelIndex &index) const
{
//	model::Model *im = const_cast<model::Model *>(static_cast<model::Model const *>(targetModel));
//	if (im){
//		return im->getTypeName(targetObject, roleByIndex(index.row()));
//	}
	return QString();
}

qrRepo::RepoApi const & PropertyEditorModel::getApi() const
{
//	model::Model *im = const_cast<model::Model *>(static_cast<model::Model const *>(targetModel));
//	return im->api();
	return qrRepo::RepoApi("./dummy");  // TODO: This will not work.
}

bool PropertyEditorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
//	bool ret = false;

//	if (!targetModel)
//		return false;

//	if (!targetObject.isValid())
//		return false;

//	if ((role == Qt::DisplayRole || role == Qt::EditRole ) && index.column() == 1) {
//		model::Model *im = const_cast<model::Model *>(static_cast<model::Model const *>(targetModel));
//		if (index.row() >= mPseudoAttributesCount)
//			 ret = im->setData(targetObject, value, roleByIndex(index.row()));
//		else if (index.row() == 2)
//			ret = im->setData(targetObject, value, Qt::DisplayRole);
//		else
//			ret = true;
//	}
//	else
//		ret = false;
//	if (ret)
//		dataChanged(index, index);
//	return ret;
	return false;
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

	// non-editable fields go first
	// fields order is hard-coded in data() and setData(), beware
	mFieldNames.push_front("name");
	mFieldNames.push_front("repo_id");
	mFieldNames.push_front("metatype");

	mPseudoAttributesCount = 3;
	mEditablePseudoAttributesCount = 1;

	reset();
}

const QModelIndex& PropertyEditorModel::getModelIndex() const
{
	return targetObject;
}

QAbstractItemModel* PropertyEditorModel::getTargetModel() const
{
	return targetModel;
}

