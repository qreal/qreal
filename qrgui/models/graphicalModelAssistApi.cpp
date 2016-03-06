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

#include "graphicalModelAssistApi.h"

#include <QtCore/QUuid>

#include <qrkernel/exception/exception.h>

using namespace qReal;
using namespace models;
using namespace models::details;

GraphicalModelAssistApi::GraphicalModelAssistApi(
		GraphicalModel &graphicalModel
		, GraphicalPartModel &graphicalPartModel
		, const EditorManagerInterface &editorManagerInterface
		)
	: mGraphicalModel(graphicalModel)
	, mModelsAssistApi(graphicalModel, editorManagerInterface)
	, mGraphicalPartModel(graphicalPartModel)
{
	connect(&graphicalModel, &GraphicalModel::elementAdded, this, &GraphicalModelAssistApi::elementAdded);
	connect(&graphicalModel, &GraphicalModel::nameChanged, this, &GraphicalModelAssistApi::nameChanged);
}

const EditorManagerInterface &GraphicalModelAssistApi::editorManagerInterface() const
{
	return mModelsAssistApi.editorManagerInterface();
}

const qrRepo::GraphicalRepoApi &GraphicalModelAssistApi::graphicalRepoApi() const
{
	return mGraphicalModel.api();
}

qrRepo::GraphicalRepoApi &GraphicalModelAssistApi::mutableGraphicalRepoApi() const
{
	return mGraphicalModel.mutableApi();
}

Id GraphicalModelAssistApi::logicalId(const Id &elem) const
{
	return mGraphicalModel.data(indexById(elem), roles::logicalIdRole).value<Id>();
}

IdList GraphicalModelAssistApi::graphicalIdsByLogicalId(const Id &logicalId) const
{
	IdList result;
	QList<QPersistentModelIndex> indexes = mGraphicalModel.indexesWithLogicalId(logicalId);
	foreach (const QPersistentModelIndex &index, indexes) {
		result.append(idByIndex(index));
	}

	return result;
}

Id GraphicalModelAssistApi::createElement(const Id &parent, const Id &type)
{
	Q_ASSERT(type.idSize() == 3);
	Q_ASSERT(parent.idSize() == 4);

	const Id newElementId = type.sameTypeId();
	const QString elementFriendlyName = mModelsAssistApi.editorManagerInterface().friendlyName(type);

	const int isEdge = mModelsAssistApi.editorManagerInterface()
			.isNodeOrEdge(newElementId.editor(), newElementId.element());

	ElementInfo newElement{newElementId, Id(), Id(), parent, {{"name", elementFriendlyName}}, {}, Id(), isEdge == -1};
	mGraphicalModel.addElementToModel(newElement);
	return newElementId;
}

Id GraphicalModelAssistApi::createElement(const Id &parent, const Id &id
		, bool isFromLogicalModel, const QString &name
		, const QPointF &position, const Id &preferedLogicalId)
{
	return mModelsAssistApi.createElement(parent, id, preferedLogicalId, isFromLogicalModel, name, position);
}

void GraphicalModelAssistApi::createElements(QList<ElementInfo> &elements)
{
	mGraphicalModel.addElementsToModel(elements);
}

Id GraphicalModelAssistApi::copyElement(const Id &source)
{
	return mGraphicalModel.mutableApi().copy(source);
}

Id GraphicalModelAssistApi::parent(const Id &element) const
{
	return mGraphicalModel.api().parent(element);
}

IdList GraphicalModelAssistApi::children(const Id &element) const
{
	return mGraphicalModel.api().children(element);
}

void GraphicalModelAssistApi::changeParent(const Id &element, const Id &parent, const QPointF &position)
{
	mGraphicalModel.changeParent(mGraphicalModel.indexById(element), mGraphicalModel.indexById(parent), position);
}

void GraphicalModelAssistApi::copyProperties(const Id &dest, const Id &src)
{
	mGraphicalModel.mutableApi().copyProperties(dest, src);
}

QMap<QString, QVariant> GraphicalModelAssistApi::properties(const Id &id)
{
	return mGraphicalModel.mutableApi().properties(id);
}

void GraphicalModelAssistApi::setProperties(const Id &id, QMap<QString, QVariant> const &properties)
{
	mGraphicalModel.mutableApi().setProperties(id, properties);
}

void GraphicalModelAssistApi::stackBefore(const Id &element, const Id &sibling)
{
	mModelsAssistApi.stackBefore(element, sibling);
}

void GraphicalModelAssistApi::setConfiguration(const Id &elem, const QPolygon &newValue)
{
	mModelsAssistApi.setProperty(elem, QVariant(newValue), roles::configurationRole);
}

QPolygon GraphicalModelAssistApi::configuration(const Id &elem) const
{
	return mModelsAssistApi.property(elem, roles::configurationRole).value<QPolygon>();
}

void GraphicalModelAssistApi::setPosition(const Id &elem, const QPointF &newValue)
{
	mModelsAssistApi.setProperty(elem, QVariant(newValue), roles::positionRole);
}

QPointF GraphicalModelAssistApi::position(const Id &elem) const
{
	return mModelsAssistApi.property(elem, roles::positionRole).value<QPointF>();
}

void GraphicalModelAssistApi::setToPort(const Id &elem, const qreal &newValue)
{
	mModelsAssistApi.setProperty(elem, QVariant(newValue), roles::toPortRole);
}

qreal GraphicalModelAssistApi::toPort(const Id &elem) const
{
	return mModelsAssistApi.property(elem, roles::toPortRole).value<qreal>();
}

void GraphicalModelAssistApi::setFromPort(const Id &elem, const qreal &newValue)
{
	mModelsAssistApi.setProperty(elem, QVariant(newValue), roles::fromPortRole);
}

qreal GraphicalModelAssistApi::fromPort(const Id &elem) const
{
	return mModelsAssistApi.property(elem, roles::fromPortRole).value<qreal>();
}

void GraphicalModelAssistApi::setName(const Id &elem, const QString &newValue)
{
	mModelsAssistApi.setProperty(elem, QVariant(newValue), Qt::DisplayRole);
}

QString GraphicalModelAssistApi::name(const Id &elem) const
{
	return mModelsAssistApi.property(elem, Qt::DisplayRole).value<QString>();
}

void GraphicalModelAssistApi::setToolTip(const Id &elem, const QString &newValue)
{
	mModelsAssistApi.setProperty(elem, QVariant(newValue), Qt::ToolTipRole);
}

QString GraphicalModelAssistApi::toolTip(const Id &elem) const
{
	return mModelsAssistApi.property(elem, Qt::ToolTipRole).value<QString>();
}

bool GraphicalModelAssistApi::isGraphicalId(const Id &id) const
{
	return indexById(id) != QModelIndex();
}

IdList GraphicalModelAssistApi::temporaryRemovedLinksFrom(const Id &elem) const
{
	return graphicalRepoApi().temporaryRemovedLinksAt(elem, "from");
}

IdList GraphicalModelAssistApi::temporaryRemovedLinksTo(const Id &elem) const
{
	return graphicalRepoApi().temporaryRemovedLinksAt(elem, "to");
}

IdList GraphicalModelAssistApi::temporaryRemovedLinksNone(const Id &elem) const
{
	return graphicalRepoApi().temporaryRemovedLinksAt(elem, QString());
}

void GraphicalModelAssistApi::removeTemporaryRemovedLinks(const Id &elem)
{
	mutableGraphicalRepoApi().removeTemporaryRemovedLinks(elem);
}

void GraphicalModelAssistApi::setTo(const Id &elem, const Id &newValue)
{
	mModelsAssistApi.setTo(elem, newValue);
}

Id GraphicalModelAssistApi::to(const Id &elem) const
{
	return mModelsAssistApi.to(elem);
}

void GraphicalModelAssistApi::setFrom(const Id &elem, const Id &newValue)
{
	mModelsAssistApi.setFrom(elem, newValue);
}

Id GraphicalModelAssistApi::from(const Id &elem) const
{
	return mModelsAssistApi.from(elem);
}

QModelIndex GraphicalModelAssistApi::indexById(const Id &id) const
{
	return mModelsAssistApi.indexById(id);
}

Id GraphicalModelAssistApi::idByIndex(const QModelIndex &index) const
{
	return mModelsAssistApi.idByIndex(index);
}

QPersistentModelIndex GraphicalModelAssistApi::rootIndex() const
{
	return mModelsAssistApi.rootIndex();
}

Id GraphicalModelAssistApi::rootId() const
{
	return mGraphicalModel.rootId();
}

bool GraphicalModelAssistApi::hasRootDiagrams() const
{
	return mModelsAssistApi.hasRootDiagrams();
}

int GraphicalModelAssistApi::childrenOfRootDiagram() const
{
	return mModelsAssistApi.childrenOfRootDiagram();
}

int GraphicalModelAssistApi::childrenOfDiagram(const Id &parent) const
{
	return mModelsAssistApi.childrenOfDiagram(parent);
}

void GraphicalModelAssistApi::removeElement(const Id &graphicalId)
{
	const QPersistentModelIndex index = indexById(graphicalId);
	if (graphicalRepoApi().exist(graphicalId) && index.isValid()) {
		mGraphicalModel.removeRow(index.row(), index.parent());
	}
}

bool GraphicalModelAssistApi::hasLabel(const Id &graphicalId, int index)
{
	return mGraphicalPartModel.findIndex(graphicalId, index).isValid();
}

void GraphicalModelAssistApi::createLabel(
		const Id &graphicalId
		, int index
		, const QPointF &position
		, const QSizeF &size
		)
{
	const QModelIndex modelIndex = mGraphicalPartModel.addGraphicalPart(graphicalId, index);
	mGraphicalPartModel.setData(modelIndex, position, GraphicalPartModel::positionRole);

	QPolygonF configuration;
	configuration.append(QPointF(size.width(), size.height()));
	mGraphicalPartModel.setData(modelIndex, configuration, GraphicalPartModel::configurationRole);
}

void GraphicalModelAssistApi::setLabelPosition(const Id &graphicalId, int index, const QPointF &position)
{
	const QModelIndex modelIndex = mGraphicalPartModel.findIndex(graphicalId, index);
	mGraphicalPartModel.setData(modelIndex, position, GraphicalPartModel::positionRole);
}

void GraphicalModelAssistApi::setLabelSize(const Id &graphicalId, int index, const QSizeF &size)
{
	const QModelIndex modelIndex = mGraphicalPartModel.findIndex(graphicalId, index);

	QPolygonF configuration;
	configuration.append(QPointF(size.width(), size.height()));
	mGraphicalPartModel.setData(modelIndex, configuration, GraphicalPartModel::configurationRole);
}

QPointF GraphicalModelAssistApi::labelPosition(const Id &graphicalId, int index) const
{
	const QModelIndex modelIndex = mGraphicalPartModel.findIndex(graphicalId, index);
	return modelIndex.data(GraphicalPartModel::positionRole).toPointF();
}

QSizeF GraphicalModelAssistApi::labelSize(const Id &graphicalId, int index) const
{
	const QModelIndex modelIndex = mGraphicalPartModel.findIndex(graphicalId, index);
	const QPolygonF configuration = modelIndex.data(GraphicalPartModel::configurationRole).value<QPolygonF>();
	return QSizeF(configuration.at(0).x(), configuration.at(0).y());
}
