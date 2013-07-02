#pragma once

#include "abstractCommand.h"
#include "../../view/editorView.h"

namespace qReal
{
namespace commands
{

/// A base for all commands using concrete element instance on some scene
class ElementCommand : public AbstractCommand
{
public:
	ElementCommand(EditorViewScene const *scene, Id const &id);
	virtual ~ElementCommand() {}

	EditorViewScene const *scene() const;
	Id elementId() const;
	void setScene(EditorViewScene const *scene);
	void setId(Id const &id);

protected:
	/// Called when our element instance possibly changed
	virtual bool reinitElement();

	virtual bool execute();
	virtual bool restoreState();

	Element *elementById(Id const &id);

	Element *mElement;
	EditorViewScene const *mScene;
	Id mId;
};

}
}
