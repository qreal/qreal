#pragma once

#include "editor/commands/nodeElementCommand.h"
#include "editor/commands/resizeCommand.h"

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
