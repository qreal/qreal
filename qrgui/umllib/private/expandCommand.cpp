#include "expandCommand.h"

namespace qReal
{
namespace commands
{

ExpandCommand::ExpandCommand(NodeElement const *node)
	: NodeElementCommand(dynamic_cast<EditorViewScene *>(node->scene()), node->id())
	, mResizeCommand(NULL)
{
}

ExpandCommand::~ExpandCommand()
{
	delete mResizeCommand;
}

bool ExpandCommand::execute()
{
	NodeElementCommand::execute();
	initRenderedDiagram();
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
	NodeElement *parent = dynamic_cast<NodeElement *>(mNode->parentItem());
	if (parent) {
		parent->resize();
	}
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
		mScene->resolveOverlaps(mNode, mNode->pos(), mNode->boundingRect().bottomRight(), mShifting);
	} else {
		mScene->returnElementsToOldPositions(mShifting);
	}
}

void ExpandCommand::initRenderedDiagram()
{
	if (mNode) {
		mNode->initRenderedDiagram();
	}
}

}
}
