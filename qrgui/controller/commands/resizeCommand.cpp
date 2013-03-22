#include "resizeCommand.h"

using namespace qReal::commands;

ResizeCommand::ResizeCommand(EditorViewScene const *scene, Id const &id
		, QRectF const &oldGeometry, QRectF const &newGeometry)
	: NodeElementCommand(scene, id)
	, mOldGeometry(oldGeometry)
	, mNewGeometry(newGeometry)
{
}

ResizeCommand::ResizeCommand(EditorView const *view, Id const &id
		, QRectF const &oldGeometry, QRectF const &newGeometry)
	: NodeElementCommand(view->editorViewScene(), id)
	, mOldGeometry(oldGeometry)
	, mNewGeometry(newGeometry)
{
}

bool ResizeCommand::execute()
{
	NodeElementCommand::execute();
	resize(mNewGeometry);
	return true;
}

bool ResizeCommand::restoreState()
{
	NodeElementCommand::restoreState();
	resize(mOldGeometry);
	return true;
}

void ResizeCommand::resize(QRectF const &geometry)
{
	if (mNode) {
		ResizeHandler handler(mNode);
		handler.resize(geometry.translated(-geometry.topLeft()), geometry.topLeft());
	}
}
