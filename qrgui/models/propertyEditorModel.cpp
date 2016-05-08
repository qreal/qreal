/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "propertyEditorModel.h"

#include <qrkernel/exception/exception.h>
#include <qrkernel/definitions.h>

using namespace qReal;

PropertyEditorModel::PropertyEditorModel(
		const qReal::EditorManagerInterface &editorManagerInterface
		, QObject *parent
		)
	: QAbstractTableModel(parent)
	, mTargetLogicalModel(nullptr)
	, mTargetGraphicalModel(nullptr)
	, mEditorManagerInterface(editorManagerInterface)
{
	mField = new Field("start", logicalAttribute, 265, nullptr);
}

int PropertyEditorModel::rowCount(const QModelIndex&) const
{
	return mFields.size();
}

int PropertyEditorModel::columnCount(const QModelIndex&) const
{
	return mField->childCount();
}

int PropertyEditorModel::countOfChilds(const QModelIndex &index) const
{
	int row = index.row();
	Field* temp = mField->child(index.row());
	int result = mField->numberOfChilds(temp);
	auto check = mField->getChilds(temp);

	return result;
}

Qt::ItemFlags PropertyEditorModel::flags(const QModelIndex &index) const
{
	// Property names
	if (index.column() == 0)
		return Qt::ItemIsEnabled;

	switch (mFields[index.row()]->attributeClass()) {
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

QVariant PropertyEditorModel::data(const QModelIndex &index, int role) const
{
	if (!isValid()) {
		return QVariant();
	}


	int row = index.row();
	int column = index.column();
	Field* child;

	if (column != 0) {

		Field* parent = mField->child(index.row());
		QList<Field*> childs = mField->getChilds(parent);

		for (int i = 0; i < childs.count(); ++i) {
			if (i == column - 1) {
				child = childs.at(i);
				break;
			}
		}


	} else {
		Field* parent = mField->child(index.row());
		child = parent;
	}

	//int asdf = index.
//	auto first = mFields[index.row()];
//	auto fff = mFields[index.row()]->fieldName();

//	auto second = mField->child(index.row());

//	Field* qwe = mField->child(index.row());
//	auto check = qwe->child(index.row());
//	int qq = qwe->childCount();


	if (role == Qt::ToolTipRole) {
		if (index.column() == 0) {
			const Id id = mTargetLogicalObject.data(roles::idRole).value<Id>();
			const QString description = mEditorManagerInterface.propertyDescription(id, child->fieldName());
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

	if (index.column() >= 0) {
		const Id id = mTargetLogicalObject.data(roles::idRole).value<Id>();
//		const QString displayedName = mEditorManagerInterface.propertyDisplayedName(id, mFields[index.row()]->fieldName());
		const QString displayedName = mEditorManagerInterface.propertyDisplayedName(id, child->fieldName());

		return displayedName.isEmpty() ? child->fieldName() : displayedName;
	}
//	} else  {
////		switch (mFields[index.row()]->attributeClass()) {
//		switch (child->attributeClass()) {

//		case logicalAttribute: {
//			return mTargetLogicalObject.data(child->role()).toString();
//		}
//		case graphicalAttribute:
//			return mTargetGraphicalObject.data(child->role());
//		case graphicalIdPseudoattribute:
//			return mTargetGraphicalObject.data(roles::idRole).value<Id>().id();
//		case logicalIdPseudoattribute:
//			return mTargetLogicalObject.data(roles::idRole).value<Id>().id();
//		case metatypePseudoattribute: {
//			const Id id = mTargetLogicalObject.data(roles::idRole).value<Id>();
//			return QVariant(id.editor() + "/" + id.diagram() + "/" + id.element());
//		}
//		case namePseudoattribute: {
//			return mTargetLogicalObject.data(Qt::DisplayRole);
//		}
//		default:
//			return QVariant();
//		}
//	}
	return QVariant();

}

bool PropertyEditorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	bool modelChanged = true;

	if (!isValid()) {
		return false;
	}

	if ((role == Qt::DisplayRole || role == Qt::EditRole) && index.column() == 1) {
		switch (mFields[index.row()]->attributeClass()) {
		case logicalAttribute: {
			mTargetLogicalModel->setData(mTargetLogicalObject, value, mFields[index.row()]->role());
			break;
		}
		case graphicalAttribute:
			mTargetGraphicalModel->setData(mTargetGraphicalObject, value, mFields[index.row()]->role());
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

bool PropertyEditorModel::enumEditable(const QModelIndex &index) const
{
	if (!index.isValid()) {
		return false;
	}

	const AttributeClassEnum attrClass = mFields[index.row()]->attributeClass();
	// metatype, ids and name are definitely not enums
	if (attrClass != logicalAttribute && attrClass != graphicalAttribute) {
		return false;
	}

	const Id id = attrClass == logicalAttribute
			? mTargetLogicalObject.data(roles::idRole).value<Id>()
			: mTargetGraphicalObject.data(roles::idRole).value<Id>();

	return mEditorManagerInterface.isEnumEditable(id, mFields[index.row()]->fieldName());
}

QList<QPair<QString, QString>> PropertyEditorModel::enumValues(const QModelIndex &index) const
{
	if (!index.isValid()) {
		return {};
	}

	const AttributeClassEnum attrClass = mFields[index.row()]->attributeClass();
	// metatype, ids and name are definitely not enums
	if (attrClass != logicalAttribute && attrClass != graphicalAttribute) {
		return {};
	}

	const Id id = attrClass == logicalAttribute
			? mTargetLogicalObject.data(roles::idRole).value<Id>()
			: mTargetGraphicalObject.data(roles::idRole).value<Id>();

	/// @todo: null id must not be met here but for some reason sometimes it happens.
	/// This is pretty strange because mTargetLogicalObject without manual modification
	/// becomes invalid index.
	return id.isNull()
			? QList<QPair<QString, QString>>()
			: mEditorManagerInterface.enumValues(id, mFields[index.row()]->fieldName());
}

void PropertyEditorModel::rereadData(const QModelIndex &topLeftIndex, const QModelIndex &bottomRightIndex)
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
		connect(mTargetLogicalModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &))
				, this, SLOT(rereadData(const QModelIndex &, const QModelIndex &)));

	// At the moment property editor does not show graphical properties at all.
	// If this should happen then dataChanged() signal of graphical model should be connected here too.
	// WARNING: This should not be done before rewriting property editor model completely.
	// Connecting graphical model here will drop QReal performance dramatically. This was checked on sad experience.
	// For each modification in graphical model setting arbitrary property (position of node, configuration of edge)
	// will cause full properties list rereading.
}

void PropertyEditorModel::setModelIndexes(const QModelIndex &logicalModelIndex
		, const QModelIndex &graphicalModelIndex)
{
	beginResetModel();
	mFields.clear();
	endResetModel();

	mTargetLogicalObject = logicalModelIndex;
	mTargetGraphicalObject = graphicalModelIndex;

	if (!isValid()) {
		return;
	}

	const Id logicalId = mTargetLogicalObject.data(roles::idRole).value<Id>();

	if (logicalModelIndex != QModelIndex()) {
		const QStringList logicalProperties = mEditorManagerInterface.propertyNames(logicalId.type());
		int role = roles::customPropertiesBeginRole;

		foreach (QString property, logicalProperties) {
			mFields.append(new Field(property, logicalAttribute, role, nullptr));
			++role;
		}

		QStringList logPropertiesClone = logicalProperties;
		int i = 0;
		while (logPropertiesClone.size() > 0) {
			QString temp = logPropertiesClone.takeAt(0);
			if (temp.contains("!")) {
				int first = temp.indexOf("!");
				QString begin = temp.mid(0, first);
				mField->appendChild(new Field(begin, logicalAttribute, role, nullptr));

				auto two = mField->child(i);

				QString end = temp.mid(first + 1);
				mField->appendChild(new Field(end, logicalAttribute, role, two));

				int j = 0;
				while (j < logPropertiesClone.size()) {
					if (logPropertiesClone.at(j).mid(0, first) == begin) {
						QString newValue = logPropertiesClone.takeAt(j);
						newValue = newValue.mid(first + 1);
						mField->appendChild(new Field(newValue, logicalAttribute, role, two));
						j = 0;
					} else {
						++j;
					}
				}
				i += j + 1;
			}


		}

		int ddd = 5;
//		if (i > 0) {

////				auto one = mField[i-1];
//			auto two = mField->child(i-1);
//			mField->appendChild(new Field(property, logicalAttribute, role, two));

//		} else {
//			mField->appendChild(new Field(property, logicalAttribute, role, nullptr));
//		}
//		++i;


		// Ids and metatype commented out as they shall not be visible to user, uncomment for debugging.
//		mFields << Field(tr("Logical Id"), logicalIdPseudoattribute);
	}

	int ololo = mField->childCount();
	auto check = mField->parentItem();
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

bool PropertyEditorModel::isCurrentIndex(const QModelIndex &index) const
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
	switch (mFields[row]->attributeClass()) {
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
	return mFields[row]->role();
}

QString PropertyEditorModel::typeName(const QModelIndex &index) const
{
	Id id = idByIndex(index);
	if (id.isNull()) {
		return "";
	}
	return mEditorManagerInterface.typeName(id, mFields[index.row()]->fieldName());
}

QString PropertyEditorModel::propertyName(const QModelIndex &index) const
{
	return mFields[index.row()]->fieldName();
}

bool PropertyEditorModel::setData(const Id &id, const QString &propertyName, const QVariant &value)
{
	if (mFields.isEmpty() || idByIndex(index(0, 0)) != id) {
		return false;
	}

	for (Field* field : mFields) {
		if (field->fieldName() == propertyName) {
			setData(index(field->role(), 1), value);
			return true;
		}
	}

	return false;
}

bool PropertyEditorModel::isReference(const QModelIndex &index, const QString &propertyName)
{
	Id id = idByIndex(index);
	if (id.isNull()) {
		return false;
	}
	return mEditorManagerInterface.referenceProperties(id.type()).contains(propertyName);
}

Id PropertyEditorModel::idByIndex(const QModelIndex &index) const
{
	switch (mFields[index.row()]->attributeClass()) {
		case logicalAttribute:
			return mTargetLogicalObject.data(roles::idRole).value<Id>();
		case graphicalAttribute:
			return mTargetGraphicalObject.data(roles::idRole).value<Id>();
		default:
			return Id();
	}
}
