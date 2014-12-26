#pragma once

#include "editor/commands/elementCommand.h"

namespace qReal {
namespace commands {

/// This command selects or deselects element on the scene
class SelectElementCommand : public ElementCommand
{
public:
	/// @param shouldSelect A selection state to be setted to our element
	/// @param forceValueChange Should selection state be setted to opposite
	///        one before or no
	SelectElementCommand(EditorViewScene const *scene, Id const &id
			, bool shouldSelect = true, bool forceValueChange = false);
	SelectElementCommand(EditorView const *view, Id const &id
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
