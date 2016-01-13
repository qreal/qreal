/* Copyright 2013-2016 Dmitry Mordvinov
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

#include "removeElementsCommand.h"

#include "models/models.h"

using namespace qReal::commands;

RemoveElementsCommand::RemoveElementsCommand(const models::Models &models)
	: mLogicalApi(models.logicalModelAssistApi())
	, mGraphicalApi(models.graphicalModelAssistApi())
	, mExploser(models.exploser())
	, mImpl(models, {})
{
}

RemoveElementsCommand *RemoveElementsCommand::withItemsToDelete(const IdList &itemsToDelete)
{
	QList<ElementInfo> edges;
	QList<ElementInfo> nodes;
	// QGraphicsScene::selectedItems() returns items in no particular order,
	// so we should handle parent-child relationships manually
	for (const Id &item : itemsToDelete) {
		// The whole hierarchy will be processed recursively in appendGraphicalDelete(), so we should just consider
		// top-level parents here. Those are elements having no parent in the list.
		if (!itemsToDelete.contains(mGraphicalApi.parent(item))) {
			appendGraphicalDelete(item, nodes, edges);
		}
	}

	appendHangingEdges(nodes, edges);

	// ElementInfos must be given into implementation in reverse order (in order they will be created during undo).
	mImpl.setElements(nodes + edges);
	return this;
}

RemoveElementsCommand *RemoveElementsCommand::withLogicalItemToDelete(const qReal::Id &logicalId)
{
	QList<ElementInfo> edges;
	QList<ElementInfo> nodes;
	appendLogicalDelete(logicalId, nodes, edges);
	appendHangingEdges(nodes, edges);

	// ElementInfos must be given into implementation in reverse order (in order they will be created during undo).
	mImpl.setElements(nodes + edges);
	return this;
}

void RemoveElementsCommand::appendInfo(QList<ElementInfo> &nodes
		, QList<ElementInfo> &edges, const ElementInfo &info) const
{
	QList<ElementInfo> &target = info.isEdge() ? edges : nodes;
	if (!target.contains(info)) {
		target << info;
	}
}

void RemoveElementsCommand::appendLogicalDelete(const Id &id, QList<ElementInfo> &nodes, QList<ElementInfo> &edges)
{
	// Logical deletion is equal to all its graphical parts deletion
	const IdList graphicalIds = mGraphicalApi.graphicalIdsByLogicalId(id);

	if (graphicalIds.isEmpty()) {
		appendInfo(nodes, edges, ElementInfo(
				id
				, id
				, mLogicalApi.parent(id)
				, Id()
				, {{"name", mGraphicalApi.name(id) }}
				, {{"position", mGraphicalApi.position(id)}}
				, Id()
				, !mLogicalApi.editorManagerInterface().isGraphicalElementNode(id)
		));
		return;
	}

	for (const Id &graphicalId : graphicalIds) {
		appendGraphicalDelete(graphicalId, nodes, edges);
	}

	if (graphicalIds.size() != 1) { // else it was done in graphicalDeleteCommand()
		appendExplosionsCommands(id, nodes, edges);
	}
}

void RemoveElementsCommand::appendGraphicalDelete(const Id &id, QList<ElementInfo> &nodes, QList<ElementInfo> &edges)
{
	const Id logicalId = mGraphicalApi.logicalId(id);
	appendInfo(nodes, edges, ElementInfo(
			id
			, logicalId
			, mLogicalApi.parent(logicalId)
			, mGraphicalApi.parent(id)
			, {{"name", mGraphicalApi.name(id)}}
			, {{"position", mGraphicalApi.position(id)}}
			, Id()
			, !mLogicalApi.editorManagerInterface().isGraphicalElementNode(id)
	));

	const IdList children = mGraphicalApi.children(id);
	for (const Id &child : children) {
		appendGraphicalDelete(child, nodes, edges);
	}

	if (mGraphicalApi.graphicalIdsByLogicalId(logicalId).size() == 1) {
		appendExplosionsCommands(logicalId, nodes, edges);
	}
}

const QList<qReal::ElementInfo> &RemoveElementsCommand::results() const
{
	return mImpl.results();
}

bool RemoveElementsCommand::execute()
{
	mImpl.remove();
	return true;
}

bool RemoveElementsCommand::restoreState()
{
	mImpl.create();
	return true;
}

void RemoveElementsCommand::appendExplosionsCommands(const Id &logicalId
		, QList<ElementInfo> &nodes, QList<ElementInfo> &edges)
{
	const IdList toDelete = mExploser.elementsWithHardDependencyFrom(logicalId);
	for (const Id &logicalChild : toDelete) {
		appendLogicalDelete(logicalChild, nodes, edges);
	}

	mExploser.handleRemoveCommand(logicalId, this);
}

void RemoveElementsCommand::appendHangingEdges(QList<ElementInfo> &nodes, QList<ElementInfo> &edges)
{
	for (const ElementInfo &node : nodes) {
		const IdList linksOfCurrentElement = mLogicalApi.logicalRepoApi().links(node.id());
		for (const Id &link : linksOfCurrentElement) {
			const Id otherEntityOfLink = mLogicalApi.logicalRepoApi().otherEntityFromLink(link, node.id());
			const bool isHanging = otherEntityOfLink == Id::rootId() || contains(nodes, otherEntityOfLink);
			if (isHanging && !contains(edges, link)) {
				appendLogicalDelete(mGraphicalApi.logicalId(link), nodes, edges);
			}
		}
	}
}

bool RemoveElementsCommand::contains(QList<qReal::ElementInfo> &infos, const qReal::Id &id) const
{
	for (const ElementInfo &info : infos) {
		if (info.id() == id || info.logicalId() == id) {
			return true;
		}
	}

	return false;
}
