/* Copyright 2012-2016 QReal Research Group
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

#include "createPatternCommand.h"

#include "models/models.h"

using namespace qReal::commands;

CreatePatternCommand::CreatePatternCommand(const models::Models &models, const ElementInfo &pattern)
	: CreateElementsCommand(models, {})
{
	setElements(parse(models.logicalModelAssistApi(), models.graphicalModelAssistApi(), pattern));
}

qReal::Id CreatePatternCommand::rootId() const
{
	return mRootId;
}

QList<qReal::ElementInfo> CreatePatternCommand::parse(models::LogicalModelAssistApi &logicalApi
		, qReal::models::GraphicalModelAssistApi &graphicalApi, const qReal::ElementInfo &info)
{
	mPattern = graphicalApi.editorManagerInterface().getPatternByName(info.id().element());
	QList<qReal::ElementInfo> result;
	const QPointF size = mPattern.size();

	// Pattern nodes create may have hierarchic structure. So we must create them in correct order
	// (parent first, child after). Cycles in hierarchy and nodes with incorrect parent id are fully ignored
	QList<GroupNode> toCreate = mPattern.nodes();
	QSet<QString> consideredNodes;
	// If group node has no parent then it has 'global' one
	mCreatedNodes[QString()] = info.graphicalParent();
	bool somethingChangedThisIteration = true;
	while (!toCreate.isEmpty() && somethingChangedThisIteration) {
		somethingChangedThisIteration = false;
		for (const GroupNode &node : toCreate) {
			if (!node.parent.isEmpty() && !consideredNodes.contains(node.parent)) {
				continue;
			}

			const Id element(info.id().editor(), info.id().diagram(), node.type, QUuid::createUuid().toString());
			mCreatedNodes[node.id] = element;
			if (node.id == mPattern.rootNode()) {
				mRootId = element;
			}

			const QPointF globalPosition = info.position();
			const QPointF nodePos(globalPosition.x() - size.x() / 2 + node.position.x()
					, globalPosition.y() + node.position.y());
			const ElementInfo nodeInfo(element, Id(), info.logicalParent(), mCreatedNodes[node.parent]
					, {{"name", logicalApi.editorManagerInterface().friendlyName(element.type())}}
					, {{"position", nodePos}}, Id(), false);

			result << nodeInfo;
			consideredNodes << node.id;
			toCreate.removeAll(node);
			somethingChangedThisIteration = true;
		}
	}

	if (!toCreate.isEmpty()) {
		qWarning() << "Warning: inconsistent pattern" << info.id().type();
	}

	for (const GroupEdge &edge : mPattern.edges()) {
		const Id element(info.id().editor(), info.id().diagram(), edge.type, QUuid::createUuid().toString());
		const ElementInfo edgeInfo(element, Id(), info.logicalParent(), info.graphicalParent()
				, {{"name", logicalApi.editorManagerInterface().friendlyName(element.type())}}, {}, Id(), true);

		result << edgeInfo;
		mCreatedEdges << element;
	}

	return result;
}
