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

#include "selectElementCommand.h"

using namespace qReal::gui::editor::commands;

SelectElementCommand::SelectElementCommand(const EditorViewScene *scene
		, const Id &id, bool shouldSelect, bool forceValueChange)
	: ElementCommand(scene, id)
	, mNewState(shouldSelect)
	, mForceValueChange(forceValueChange)
{
}

SelectElementCommand::SelectElementCommand(const EditorView *view
		, const Id &id, bool shouldSelect, bool forceValueChange)
	: ElementCommand(&view->editorViewScene(), id)
	, mNewState(shouldSelect)
	, mForceValueChange(forceValueChange)
{
}

SelectElementCommand::~SelectElementCommand()
{
}

bool SelectElementCommand::execute()
{
	if (!ElementCommand::execute()) {
		return false;
	}

	mOldState = isSelected();
	return setSelectionState(mNewState);
}

bool SelectElementCommand::restoreState()
{
	return ElementCommand::restoreState() && setSelectionState(mOldState);
}

bool SelectElementCommand::setSelectionState(bool select)
{
	if (!mElement) {
		return false;
	}

	if (mForceValueChange) {
		mElement->setSelected(!isSelected());
	}

	mElement->setSelected(select);
	return true;
}

bool SelectElementCommand::isSelected() const
{
	return mElement && mElement->isSelected();
}
