#include "arrangeLinksCommand.h"

using namespace qReal::commands;

ArrangeLinksCommand::ArrangeLinksCommand(const EditorViewScene *scene
		, const Id &id, bool needAdjusting)
	: ElementCommand(scene, id), mNeedAdjusting(needAdjusting)
{
}

ArrangeLinksCommand::ArrangeLinksCommand(const EditorView *view
		, const Id &id, bool needAdjusting)
	: ElementCommand(&view->editorViewScene(), id), mNeedAdjusting(needAdjusting)
{
}

ArrangeLinksCommand::~ArrangeLinksCommand()
{
}

bool ArrangeLinksCommand::execute()
{
	if (!ElementCommand::execute()) {
		return false;
	}
	arrange();
	return true;
}

bool ArrangeLinksCommand::restoreState()
{
	if (!ElementCommand::restoreState()) {
		return false;
	}
	arrange();
	return true;
}

void ArrangeLinksCommand::arrange()
{
	NodeElement *node = dynamic_cast<NodeElement *>(mElement);
	EdgeElement *edge = dynamic_cast<EdgeElement *>(mElement);
	if (node) {
		arrange(node);
	}
	if (edge) {
		NodeElement *src = edge->src();
		NodeElement *dst = edge->dst();
		if (src) {
			arrange(src);
		}
		if (dst) {
			arrange(dst);
		}
	}
}

void ArrangeLinksCommand::arrange(NodeElement *node)
{
	node->arrangeLinks();
	if (mNeedAdjusting) {
		node->adjustLinks();
	}
}
