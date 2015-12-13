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

#pragma once

#include "editor/commands/elementCommand.h"

namespace qReal {
namespace gui {
namespace editor {
namespace commands {

/// This command selects or deselects element on the scene
class SelectElementCommand : public ElementCommand
{
public:
	/// @param shouldSelect A selection state to be setted to our element
	/// @param forceValueChange Should selection state be setted to opposite
	///        one before or no
	SelectElementCommand(const EditorViewScene *scene, const Id &id
			, bool shouldSelect = true, bool forceValueChange = false);
	SelectElementCommand(const EditorView *view, const Id &id
			, bool shouldSelect = true, bool forceValueChange = false);
	virtual ~SelectElementCommand();

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	bool setSelectionState(bool select);
	bool isSelected() const;

	bool mNewState;
	bool mForceValueChange;
	bool mOldState;
};

}
}
}
}
