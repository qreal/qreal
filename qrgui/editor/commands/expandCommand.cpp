#include "expandCommand.h"

namespace qReal {
namespace commands {

ExpandCommand::ExpandCommand(NodeElement const *node)
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

}
}
