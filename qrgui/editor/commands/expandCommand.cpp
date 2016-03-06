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

#include "expandCommand.h"

using namespace qReal::gui::editor::commands;

ExpandCommand::ExpandCommand(const NodeElement *node)
	: NodeElementCommand(dynamic_cast<EditorViewScene *>(node->scene()), node->id())
	, mResizeCommand(nullptr)
{
}

ExpandCommand::~ExpandCommand()
{
	delete mResizeCommand;
}

bool ExpandCommand::execute()
{
	NodeElementCommand::execute();
	delete mResizeCommand;
	mResizeCommand = new ResizeCommand(mScene, mNode->id());
	mResizeCommand->startTracking();
	QRectF newContents = mNode->contentsRect();
	if (!mNode->isExpanded()) {
		newContents.setWidth(2 * newContents.width());
		newContents.setHeight(2 * newContents.height());
	} else {
		newContents.setWidth(newContents.width() / 2);
		newContents.setHeight(newContents.height() / 2);
	}

	newContents.moveTo(mNode->pos());
	mNode->setGeometry(newContents);
	changeExpanded();
	mNode->resize();
	mResizeCommand->stopTracking();
	mResizeCommand->redo();
	return true;
}

bool ExpandCommand::restoreState()
{
	NodeElementCommand::restoreState();
	changeExpanded();
	mResizeCommand->undo();
	return true;
}

void ExpandCommand::changeExpanded()
{
	if (!mNode) {
		return;
	}
	mNode->changeExpanded();
	if (mNode->isExpanded()) {
		mShifting.clear();
		mShifting.insert(mNode->id(), QPointF(0, 0));
		mScene->resolveOverlaps(mNode, mNode->pos(), mNode->boundingRect().bottomRight(), mShifting);
	} else {
		mScene->returnElementsToOldPositions(mShifting);
	}
}
