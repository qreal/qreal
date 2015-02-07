#pragma once

#include "editor/commands/elementCommand.h"

namespace qReal {
namespace commands {

/// Arranges links of specified node element
class ArrangeLinksCommand : public ElementCommand
{
public:
	ArrangeLinksCommand(const EditorViewScene *scene, const Id &id, bool needAdjusting = false);
	ArrangeLinksCommand(const EditorView *view, const Id &id, bool needAdjusting = false);
	virtual ~ArrangeLinksCommand();

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	void arrange();
	void arrange(NodeElement *node);

	bool mNeedAdjusting;
};

}
}
