/* Copyright 2007-2016 QReal Research Group
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

#include "logicalModelAssistApi.h"

#include <QtCore/QUuid>

using namespace qReal;
using namespace models;
using namespace models::details;

LogicalModelAssistApi::LogicalModelAssistApi(LogicalModel &logicalModel
		, const EditorManagerInterface &editorManagerInterface)
	: mModelsAssistApi(logicalModel, editorManagerInterface)
	, mLogicalModel(logicalModel)
{
	connect(&logicalModel, &LogicalModel::elementAdded, this, &LogicalModelAssistApi::elementAdded);
}

LogicalModelAssistApi::~LogicalModelAssistApi()
{
}

const EditorManagerInterface &LogicalModelAssistApi::editorManagerInterface() const
{
	return mModelsAssistApi.editorManagerInterface();
}

const qrRepo::LogicalRepoApi &LogicalModelAssistApi::logicalRepoApi() const
{
	return mLogicalModel.api();
}

qrRepo::LogicalRepoApi &LogicalModelAssistApi::mutableLogicalRepoApi()
{
	return mLogicalModel.mutableApi();
}

Id LogicalModelAssistApi::createElement(const Id &parent, const Id &type)
{
	Q_ASSERT(type.idSize() == 3);
	Q_ASSERT(parent.idSize() == 4);

	const Id newElementId = type.sameTypeId();
	const QString elementFriendlyName = mModelsAssistApi.editorManagerInterface().friendlyName(type);
	const bool isEdge = mModelsAssistApi.editorManagerInterface().isNodeOrEdge(
			newElementId.editor(), newElementId.element()) == -1;
	ElementInfo newElement(newElementId, Id(), parent, Id(), {{"name", elementFriendlyName}}, {}, Id(), isEdge);
	mLogicalModel.addElementToModel(newElement);
	return newElementId;
}

Id LogicalModelAssistApi::createElement(const Id &parent, const Id &id
		, bool isFromLogicalModel, const QString &name
		, const QPointF &position, const Id &preferedLogicalId)
{
	Q_UNUSED(preferedLogicalId)
	return mModelsAssistApi.createElement(parent, id, id, isFromLogicalModel, name, position);
}

void LogicalModelAssistApi::createElements(QList<ElementInfo> &elements)
{
	mLogicalModel.addElementsToModel(elements);
}

Id LogicalModelAssistApi::parent(const Id &element) const
{
	return mLogicalModel.api().parent(element);
}

IdList LogicalModelAssistApi::children(const Id &element) const
{
	return mLogicalModel.api().children(element);
}

void LogicalModelAssistApi::changeParent(const Id &element, const Id &parent, const QPointF &position)
{
	Q_UNUSED(position);
	mLogicalModel.changeParent(mLogicalModel.indexById(element), mLogicalModel.indexById(parent), QPointF());
}

void LogicalModelAssistApi::addExplosion(const Id &source, const Id &destination)
{
	mLogicalModel.mutableApi().addExplosion(source, destination);
}

void LogicalModelAssistApi::removeExplosion(const Id &source, const Id &destination)
{
	mLogicalModel.mutableApi().removeExplosion(source, destination);
}

void LogicalModelAssistApi::setPropertyByRoleName(const Id &elem, const QVariant &newValue, const QString &roleName)
{
	const int roleIndex = mModelsAssistApi.roleIndexByName(elem, roleName);
	if (roleIndex < roles::customPropertiesBeginRole) {
		return;
	}
	mModelsAssistApi.setProperty(elem, newValue, roleIndex);
}

QVariant LogicalModelAssistApi::propertyByRoleName(const Id &elem, const QString &roleName) const
{
	int roleIndex = mModelsAssistApi.roleIndexByName(elem, roleName);
	if (roleIndex < roles::customPropertiesBeginRole)
		return "";
	return mModelsAssistApi.property(elem, roleIndex);
}

bool LogicalModelAssistApi::isLogicalId(const Id &id) const
{
	return mModelsAssistApi.indexById(id) != QModelIndex();
}

void LogicalModelAssistApi::setName(const Id &elem, const QString &newValue)
{
	mModelsAssistApi.setProperty(elem, QVariant(newValue), Qt::DisplayRole);
}

QString LogicalModelAssistApi::name(const Id &elem) const
{
	return mModelsAssistApi.property(elem, Qt::DisplayRole).value<QString>();
}

void LogicalModelAssistApi::setTo(const Id &elem, const Id &newValue)
{
	mModelsAssistApi.setTo(elem, newValue);
}

Id LogicalModelAssistApi::to(const Id &elem) const
{
	return mModelsAssistApi.to(elem);
}

void LogicalModelAssistApi::setFrom(const Id &elem, const Id &newValue)
{
	mModelsAssistApi.setFrom(elem, newValue);
}

Id LogicalModelAssistApi::from(const Id &elem) const
{
	return mModelsAssistApi.from(elem);
}

QModelIndex LogicalModelAssistApi::indexById(const Id &id) const
{
	return mModelsAssistApi.indexById(id);
}

Id LogicalModelAssistApi::idByIndex(const QModelIndex &index) const
{
	return mModelsAssistApi.idByIndex(index);
}

QPersistentModelIndex LogicalModelAssistApi::rootIndex() const
{
	return mModelsAssistApi.rootIndex();
}

Id LogicalModelAssistApi::rootId() const
{
	return mModelsAssistApi.rootId();
}

bool LogicalModelAssistApi::hasRootDiagrams() const
{
	return mModelsAssistApi.hasRootDiagrams();
}

int LogicalModelAssistApi::childrenOfRootDiagram() const
{
	return mModelsAssistApi.childrenOfRootDiagram();
}

int LogicalModelAssistApi::childrenOfDiagram(const Id &parent) const
{
	return mModelsAssistApi.childrenOfDiagram(parent);
}

void LogicalModelAssistApi::removeReferencesTo(const Id &id)
{
	IdList backReferences = mLogicalModel.api().property(id, "backReferences").value<IdList>();

	foreach (const Id &reference, backReferences) {
		mLogicalModel.api().removeBackReference(id, reference);
		removeReference(reference, id);
	}
}

void LogicalModelAssistApi::removeReferencesFrom(const Id &id)
{
	QStringList referenceProperties = mModelsAssistApi.editorManagerInterface().referenceProperties(id.type());

	foreach (const QString &property, referenceProperties) {
		QStringList propertyValue = mLogicalModel.api().property(id, property).toString().split(','
				, QString::SkipEmptyParts);
		foreach (const QString &value, propertyValue) {
			Id idValue = Id::loadFromString(value);
			mLogicalModel.api().removeBackReference(idValue, id);
		}
	}
}

void LogicalModelAssistApi::removeReference(const Id &id, const Id &reference)
{
	QStringList referenceProperties = mModelsAssistApi.editorManagerInterface().referenceProperties(id.type());

	foreach (const QString &propertyName, referenceProperties) {
		QStringList stringData = mLogicalModel.api().property(id, propertyName).toString().split(','
				, QString::SkipEmptyParts);
		stringData.removeAll(reference.toString());
		mLogicalModel.mutableApi().setProperty(id, propertyName, stringData.join(','));
	}
}

void LogicalModelAssistApi::removeElement(const Id &logicalId)
{
	const QPersistentModelIndex index = indexById(logicalId);
	if (logicalRepoApi().exist(logicalId) && index.isValid()) {
		removeReferencesTo(logicalId);
		mLogicalModel.removeRow(index.row(), index.parent());
	}
}

QMap<Id, Version> LogicalModelAssistApi::editorVersions() const
{
	QMap<Id, Version> result;

	const QStringList metaInformationKeys = logicalRepoApi().metaInformationKeys();
	for (const Id &editor : editorManagerInterface().editors()) {
		const QString key = editor.editor() + "Version";
		if (metaInformationKeys.contains(key)) {
			result[editor] = Version::fromString(logicalRepoApi().metaInformation(key).toString());
		}
	}

	return result;
}
