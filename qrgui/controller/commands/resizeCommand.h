#pragma once

#include "nodeElementCommand.h"
#include "../../umllib/private/resizeHandler.h"

namespace qReal
{
namespace commands
{

class ResizeCommand : public NodeElementCommand
{
public:
	ResizeCommand(EditorViewScene const *scene, Id const &id
			, QRectF const &oldGeometry, QRectF const &newGeometry);
	ResizeCommand(EditorView const *view, Id const &id
			, QRectF const &oldGeometry, QRectF const &newGeometry);
	virtual ~ResizeCommand() {}

protected:
	bool execute();
	bool restoreState();

private:
	void resize(QRectF const &geometry);

	QRectF mOldGeometry;
	QRectF mNewGeometry;
};

}
}
