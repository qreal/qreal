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
#include "details/logicalModel.h"

#include <qrkernel/exception/exception.h>
#include <qrkernel/definitions.h>

using namespace qReal;

PropertyEditorModel::PropertyEditorModel(
		const qReal::EditorManagerInterface &editorManagerInterface
		, QObject *parent
		)
	: QAbstractItemModel(parent)
	, mTargetLogicalModel(nullptr)
	, mTargetGraphicalModel(nullptr)
	, mEditorManagerInterface(editorManagerInterface)
{
	mField = new Field("start", logicalAttribute, 265, nullptr);
}

int PropertyEditorModel::rowCount(const QModelIndex&) const
{
	return mField->childCount();
}

int PropertyEditorModel::columnCount(const QModelIndex&) const
{
	return mField->childCount();
}

void PropertyEditorModel::setValueForIndex(const QModelIndex &index, QString value)
{
	mField->setValue(index.row() + index.column(), value);
}

QString PropertyEditorModel::getValueFromIndex(const QModelIndex &index)
{
	Field *childItem = mField->child(index.row() + index.column());
	return childItem->value();
}

QModelIndex PropertyEditorModel::index(int row, int column, const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	Field *childItem = mField->child(row + column);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}


QModelIndex PropertyEditorModel::parent(const QModelIndex &index) const
{
	Field *childItem = static_cast<Field*>(index.internalPointer());
	Field *parentItem = childItem->parentItem();

	if (parentItem == mField->parentItem())
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}


int PropertyEditorModel::countOfChilds(const QModelIndex &index) const
{
	Field* temp = mField->child(index.row());
	return mField->numberOfChilds(temp);
}

Qt::ItemFlags PropertyEditorModel::flags(const QModelIndex &index) const
{
	if (index.column() == 0)
		return Qt::ItemIsEnabled;

	switch (mField->child(index.row())->attributeClass()) {
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

	Field* child = mField->child(index.row() + index.column());
	if (role == Qt::ToolTipRole) {
		const Id id = mTargetLogicalObject.data(roles::idRole).value<Id>();
		QString propertyName = fullPropertyName(index);

		const QString description = mEditorManagerInterface.propertyDescription(id, propertyName);
		return description;
	}

	const Id id = mTargetLogicalObject.data(roles::idRole).value<Id>();
	const QString displayedName = mEditorManagerInterface.propertyDisplayedName(id, child->fieldName());

	return displayedName.isEmpty() ? child->fieldName() : displayedName;

}

bool PropertyEditorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	bool modelChanged = true;

	if (!isValid()) {
		return false;
	}

	Field* child = mField->child(index.row() + index.column());

	if (role == Qt::DisplayRole || role == Qt::EditRole) {
		switch (child->attributeClass()) {
		case logicalAttribute: {
			mTargetLogicalModel->setData(mTargetLogicalObject, value, child->role());
			break;
		}
		case graphicalAttribute:
			mTargetGraphicalModel->setData(mTargetGraphicalObject, value, child->role());
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

	const AttributeClassEnum attrClass = mField->child(index.row())->attributeClass();
	// metatype, ids and name are definitely not enums
	if (attrClass != logicalAttribute && attrClass != graphicalAttribute) {
		return false;
	}

	QString propertyName = fullPropertyName(index);
	if (propertyName == "Error") {
		return {};
	}

	const Id id = attrClass == logicalAttribute
			? mTargetLogicalObject.data(roles::idRole).value<Id>()
			: mTargetGraphicalObject.data(roles::idRole).value<Id>();

	return mEditorManagerInterface.isEnumEditable(id, propertyName);
}

QList<QPair<QString, QString>> PropertyEditorModel::enumValues(const QModelIndex &index) const
{
	if (!index.isValid()) {
		return {};
	}

	const AttributeClassEnum attrClass = mField->child(index.row())->attributeClass();
	// metatype, ids and name are definitely not enums
	if (attrClass != logicalAttribute && attrClass != graphicalAttribute) {
		return {};
	}

	const Id id = attrClass == logicalAttribute
			? mTargetLogicalObject.data(roles::idRole).value<Id>()
			: mTargetGraphicalObject.data(roles::idRole).value<Id>();

	QString propertyName = fullPropertyName(index);

	if (propertyName == "Error") {
		return {};
	}

	/// @todo: null id must not be met here but for some reason sometimes it happens.
	/// This is pretty strange because mTargetLogicalObject without manual modification
	/// becomes invalid index.
	return id.isNull()
			? QList<QPair<QString, QString>>()
			: mEditorManagerInterface.enumValues(id, propertyName);
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
	mField = new Field("start", logicalAttribute, -1, nullptr);
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
	mField = new Field("start", logicalAttribute, -1, nullptr);
	endResetModel();

	mTargetLogicalObject = logicalModelIndex;
	mTargetGraphicalObject = graphicalModelIndex;

	if (!isValid()) {
		return;
	}

	const Id logicalId = mTargetLogicalObject.data(roles::idRole).value<Id>();
	const QString dynamicProperties = dynamic_cast<models::details::LogicalModel *>(mTargetLogicalModel)->
			logicalModelAssistApi().logicalRepoApi().stringProperty(logicalId, "dynamicProperties");

	if (logicalModelIndex != QModelIndex()) {
		const QStringList logicalProperties = mEditorManagerInterface.propertyNames(logicalId.type());

		int role = roles::customPropertiesBeginRole;
		QStringList cloneWithRoles;
		QStringList cloneWithPure;

		for (const QString &prop : logicalProperties) {
			if (prop.contains("!")) {
				cloneWithRoles.append(prop);
			} else {
				cloneWithPure.append(prop);
			}
		}

		int i = 0;
		role = roles::customPropertiesBeginRole;
		while (cloneWithRoles.size() > 0) {
			QString roleName = cloneWithRoles.takeAt(0);
			int first = roleName.indexOf("!");
			QString beginPartName = roleName.mid(0, first);
			mField->appendChild(new Field(beginPartName, logicalAttribute, -1, nullptr));
			auto parent = mField->child(i);

			QString endPartName = temp.mid(first + 1);
			mField->appendChild(new Field(endPartName, logicalAttribute, role, parent));
			++i;
			QString val = mTargetLogicalObject.data(role).toString();
			mField->setValue(i, val);
			++role;

			int j = 0;
			while (j < cloneWithRoles.size()) {
				if (cloneWithRoles.at(j).mid(0, first) == beginPartName) {
					QString roleName = cloneWithRoles.takeAt(j);
					roleName = roleName.mid(first + 1);
					mField->appendChild(new Field(roleName, logicalAttribute, role, parent));
					++i;
					QString value = mTargetLogicalObject.data(role).toString();
					mField->setValue(i, value);
					++role;
					j = 0;
				} else {
					++j;
				}
			}

			++i;
		}

		while (cloneWithPure.size()  > 0) {
			QString roleName = cloneWithPure.takeAt(0);
			mField->appendChild(new Field(roleName, logicalAttribute, role, nullptr));
			QString val = mTargetLogicalObject.data(role).toString();
			mField->setValue(i, val);
			++i;
			++role;
		}

		if (!dynamicProperties.isEmpty()) {
			QDomDocument dynamProperties;
			dynamProperties.setContent(dynamicProperties);
			for (QDomElement element = dynamProperties.firstChildElement("properties").firstChildElement("property")
					; !element.isNull()
					; element = element.nextSiblingElement("property"))
			{
				mField->appendChild(new Field(element.attribute("text"), logicalAttribute, role, nullptr));
				++role;
			}
		}

	}

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
	switch (mField->child(row)->attributeClass()) {
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
	return mField->child(row)->role();
}

QString PropertyEditorModel::fullPropertyName(const QModelIndex &index) const
{
	Field* child = mField->child(index.row() + index.column());
	Field* parent = child->parentItem();

	QString childName = child->fieldName();

	QString propertyName = "";
	if (parent) {
		QString parentName = parent->fieldName();
		propertyName = parentName + "!" + childName;
	} else {
		if (child->role() == -1) {
			return "Error";
		}

		propertyName = childName;
	}

	return propertyName;
}

QString PropertyEditorModel::typeName(const QModelIndex &index) const
{
	Id id = idByIndex(index);
	if (id.isNull()) {
		return "";
	}

	const QString propertyName = fullPropertyName(index);
	return mEditorManagerInterface.typeName(id, propertyName);
}

QString PropertyEditorModel::propertyName(const QModelIndex &index) const
{
	QString fieldName = mField->child(index.row())->fieldName();
	const Id logicalId = mTargetLogicalObject.data(roles::idRole).value<Id>();
	const QString dynamicProperties = dynamic_cast<models::details::LogicalModel *>(mTargetLogicalModel)->
		logicalModelAssistApi().logicalRepoApi().stringProperty(logicalId, "dynamicProperties");

	if (!dynamicProperties.isEmpty()) {
			int propertiesCount = mEditorManagerInterface.propertyNames(logicalId.type()).count();
			QDomDocument dynamProperties;
			dynamProperties.setContent(dynamicProperties);
			int i = 0;
			for (QDomElement element
					= dynamProperties.firstChildElement("properties").firstChildElement("property");
					!element.isNull();
					element = element.nextSiblingElement("property"))
			{
				if (i == index.row() - propertiesCount) {
					fieldName = element.attribute("textBinded");
					break;
				}
				++i;
			}
		}

	return fieldName;
}

bool PropertyEditorModel::setData(const Id &id, const QString &propertyName, const QVariant &value)
{
	if (mField->childCount() == 0 || idByIndex(index(0, 0)) != id) {
		return false;
	}

	int countOfChilds = mField->childCount();

	for (int i = 0; i < countOfChilds; ++i) {
		Field* child = mField->child(i);
		Field* parent = child->parentItem();
		if (child->fieldName() == propertyName) {
			setData(index(child->role(), 1), value);
			return true;
		} else if (parent && (parent->fieldName() + "!" + child->fieldName() == propertyName)) {
			setData(index(child->role(), 1), value);
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
	Field* child = mField->child(index.row() + index.column());

	switch (child->attributeClass()) {
		case logicalAttribute:
			return mTargetLogicalObject.data(roles::idRole).value<Id>();
		case graphicalAttribute:
			return mTargetGraphicalObject.data(roles::idRole).value<Id>();
		default:
			return Id();
	}
}
