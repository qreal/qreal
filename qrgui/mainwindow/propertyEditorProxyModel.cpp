/** @file propertyeditorproxymodel.cpp
*	@brief Property editor model
* */

#include "propertyEditorProxyModel.h"

#include <qrkernel/exception/exception.h>
#include <qrkernel/definitions.h>

using namespace qReal;

PropertyEditorModel::PropertyEditorModel(
		qReal::EditorManagerInterface const &editorManagerInterface
		, QObject *parent
		)
		: QAbstractTableModel(parent)
		, mTargetLogicalModel(nullptr)
		, mTargetGraphicalModel(nullptr)
		, mEditorManagerInterface(editorManagerInterface)
{
}

int PropertyEditorModel::rowCount(const QModelIndex&) const
{
	return mFields.size();
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

	switch (mFields[index.row()].attributeClass) {
	case logicalAttribute:
	case graphicalAttribute:
	case namePseudoattribute:
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	case graphicalIdPseudoattribute:
	case logicalIdPseudoattribute:
	case metatypePseudoattribute:
	default:
		return Qt::NoItemFlags;
	}
}

QVariant PropertyEditorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
		return QString(section == 1 ? "value" : "name");
	else
		return QVariant();
}

QVariant PropertyEditorModel::data(QModelIndex const &index, int role) const
{
	if (!isValid()) {
		return QVariant();
	}

	if (role == Qt::ToolTipRole) {
		if (index.column() == 0) {
			Id const id = mTargetLogicalObject.data(roles::idRole).value<Id>();
			QString const description = mEditorManagerInterface.propertyDescription(id, mFields[index.row()].fieldName);
			if (!description.isEmpty()) {
				return "<body>" + description;
			} else {
				return QVariant();
			}
		} else if (index.column() == 1) {
			return data(index, Qt::DisplayRole);
		} else {
			return QVariant();
		}
	}

	if (role != Qt::DisplayRole) {
		return QVariant();
	}

	if (index.column() == 0) {
		Id const id = mTargetLogicalObject.data(roles::idRole).value<Id>();
		QString const displayedName = mEditorManagerInterface.propertyDisplayedName(id, mFields[index.row()].fieldName);
		return displayedName.isEmpty() ? mFields[index.row()].fieldName : displayedName;
	} else if (index.column() == 1) {
		switch (mFields[index.row()].attributeClass) {
		case logicalAttribute: {
			return mTargetLogicalObject.data(mFields[index.row()].role).toString();
		}
		case graphicalAttribute:
			return mTargetGraphicalObject.data(mFields[index.row()].role);
		case graphicalIdPseudoattribute:
			return mTargetGraphicalObject.data(roles::idRole).value<Id>().id();
		case logicalIdPseudoattribute:
			return mTargetLogicalObject.data(roles::idRole).value<Id>().id();
		case metatypePseudoattribute: {
			Id const id = mTargetLogicalObject.data(roles::idRole).value<Id>();
			return QVariant(id.editor() + "/" + id.diagram() + "/" + id.element());
		}
		case namePseudoattribute: {
			return mTargetLogicalObject.data(Qt::DisplayRole);
		}
		default:
			return QVariant();
		}
	} else {
		return QVariant();
	}
}

bool PropertyEditorModel::setData(QModelIndex const &index, QVariant const &value, int role)
{
	bool modelChanged = true;

	if (!isValid()) {
		return false;
	}

	if ((role == Qt::DisplayRole || role == Qt::EditRole) && index.column() == 1) {
		switch (mFields[index.row()].attributeClass) {
		case logicalAttribute: {
			mTargetLogicalModel->setData(mTargetLogicalObject, value, mFields[index.row()].role);
			break;
		}
		case graphicalAttribute:
			mTargetGraphicalModel->setData(mTargetGraphicalObject, value, mFields[index.row()].role);
			break;
		case namePseudoattribute:
			mTargetLogicalModel->setData(mTargetLogicalObject, value, Qt::DisplayRole);
			break;
		default:
			modelChanged = false;
		}
	} else {
		modelChanged = false;
	}

	if (modelChanged) {
		emit dataChanged(index, index);
	}

	return modelChanged;
}

bool PropertyEditorModel::enumEditable(QModelIndex const &index) const
{
	if (!index.isValid()) {
		return false;
	}

	AttributeClassEnum const attrClass = mFields[index.row()].attributeClass;
	// metatype, ids and name are definitely not enums
	if (attrClass != logicalAttribute && attrClass != graphicalAttribute) {
		return false;
	}

	Id const id = attrClass == logicalAttribute
			? mTargetLogicalObject.data(roles::idRole).value<Id>()
			: mTargetGraphicalObject.data(roles::idRole).value<Id>();

	return mEditorManagerInterface.isEnumEditable(id, mFields[index.row()].fieldName);
}

QList<QPair<QString, QString>> PropertyEditorModel::enumValues(QModelIndex const &index) const
{
	if (!index.isValid()) {
		return {};
	}

	AttributeClassEnum const attrClass = mFields[index.row()].attributeClass;
	// metatype, ids and name are definitely not enums
	if (attrClass != logicalAttribute && attrClass != graphicalAttribute) {
		return {};
	}

	Id const id = attrClass == logicalAttribute
			? mTargetLogicalObject.data(roles::idRole).value<Id>()
			: mTargetGraphicalObject.data(roles::idRole).value<Id>();

	return mEditorManagerInterface.enumValues(id, mFields[index.row()].fieldName);
}

void PropertyEditorModel::rereadData(QModelIndex const &topLeftIndex, QModelIndex const &bottomRightIndex)
{
	emit dataChanged(topLeftIndex, bottomRightIndex);
}

void PropertyEditorModel::setSourceModels(QAbstractItemModel * const sourceLogicalModel
		, QAbstractItemModel * const sourceGraphicalModel)
{
	mTargetLogicalModel = sourceLogicalModel;
	mTargetGraphicalModel = sourceGraphicalModel;

	beginResetModel();
	mFields.clear();
	endResetModel();

	if (mTargetLogicalModel)
		connect(mTargetLogicalModel, SIGNAL(dataChanged(QModelIndex const &, QModelIndex const &))
				, this, SLOT(rereadData(QModelIndex const &, QModelIndex const &)));

	if (mTargetGraphicalModel)
		connect(mTargetGraphicalModel, SIGNAL(dataChanged(QModelIndex const &, QModelIndex const &))
				, this, SLOT(rereadData(QModelIndex const &, QModelIndex const &)));
}

void PropertyEditorModel::setModelIndexes(QModelIndex const &logicalModelIndex
		, QModelIndex const &graphicalModelIndex)
{
	beginResetModel();
	mFields.clear();
	endResetModel();

	mTargetLogicalObject = logicalModelIndex;
	mTargetGraphicalObject = graphicalModelIndex;

	if (!isValid()) {
		return;
	}

	Id const logicalId = mTargetLogicalObject.data(roles::idRole).value<Id>();

	if (logicalModelIndex != QModelIndex()) {
		QStringList const logicalProperties = mEditorManagerInterface.propertyNames(logicalId.type());
		int role = roles::customPropertiesBeginRole;
		foreach (QString property, logicalProperties) {
			mFields << Field(property, logicalAttribute, role);
			++role;
		}
		// Ids and metatype commented out as they shall not be visible to user, uncomment for debugging.
//		mFields << Field(tr("Logical Id"), logicalIdPseudoattribute);
	}

	// There are no custom attributes for graphical objects, but they shall be
	// added soon.
//	if (graphicalModelIndex != QModelIndex()) {
//		mFields << Field(tr("Graphical Id"), graphicalIdPseudoattribute);
//	}

//	mFields << Field(tr("Metatype"), metatypePseudoattribute);

	beginResetModel();
	endResetModel();
}

void PropertyEditorModel::clearModelIndexes()
{
	setModelIndexes(QModelIndex(), QModelIndex());
}

bool PropertyEditorModel::isCurrentIndex(QModelIndex const &index) const
{
	return index == mTargetLogicalObject || index == mTargetGraphicalObject;
}

bool PropertyEditorModel::isValid() const
{
	return mTargetGraphicalModel && mTargetLogicalModel
			&& (mTargetLogicalObject.isValid() || mTargetGraphicalObject.isValid());
}

QModelIndex PropertyEditorModel::modelIndex(int row) const
{
	switch (mFields[row].attributeClass) {
	case logicalAttribute:
		return mTargetLogicalObject;
	case graphicalAttribute:
		return mTargetGraphicalObject;
	default:
		throw Exception("PropertyEditorModel::modelIndex: called for incorrect field,"
				"which is not graphical nor logical attribute");
	}
	return QModelIndex();
}

int PropertyEditorModel::roleByIndex(int row) const
{
	return mFields[row].role;
}

QString PropertyEditorModel::typeName(QModelIndex const &index) const
{
	Id id = idByIndex(index);
	if (id.isNull()) {
		return "";
	}
	return mEditorManagerInterface.typeName(id, mFields[index.row()].fieldName);
}

bool PropertyEditorModel::isReference(QModelIndex const &index, QString const &propertyName)
{
	Id id = idByIndex(index);
	if (id.isNull()) {
		return false;
	}
	return mEditorManagerInterface.referenceProperties(id.type()).contains(propertyName);
}

Id PropertyEditorModel::idByIndex(QModelIndex const &index) const
{
	switch (mFields[index.row()].attributeClass) {
		case logicalAttribute:
			return mTargetLogicalObject.data(roles::idRole).value<Id>();
		case graphicalAttribute:
			return mTargetGraphicalObject.data(roles::idRole).value<Id>();
		default:
			return Id();
	}
}
