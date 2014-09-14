#pragma once

#include "controller/commands/nodeElementCommand.h"
#include "editor/private/resizeCommand.h"

namespace qReal {
namespace commands {

class ExpandCommand : public NodeElementCommand
{
public:
	ExpandCommand(NodeElement const *node);
	virtual ~ExpandCommand();

protected:
	bool execute();
	bool restoreState();

private:
	void changeExpanded();

	QMap<Id, QPointF> mShifting;
	ResizeCommand *mResizeCommand;
};

}
}
