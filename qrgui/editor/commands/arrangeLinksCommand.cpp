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

#include "arrangeLinksCommand.h"

using namespace qReal::gui::editor::commands;

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
