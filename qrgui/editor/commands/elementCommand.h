#pragma once

#include "controller/commands/abstractCommand.h"
#include "editor/editorView.h"

namespace qReal {
namespace commands {

/// A base for all commands using concrete element instance on some scene
class ElementCommand : public AbstractCommand
{
	Q_OBJECT

public:
	ElementCommand(EditorViewScene const *scene, const Id &id);
	virtual ~ElementCommand();

	EditorViewScene const *scene() const;
	Id elementId() const;
	void setScene(EditorViewScene const *scene);
	void setId(const Id &id);

protected:
	/// Called when our element instance possibly has changed
	virtual bool reinitElement();

	virtual bool execute();
	virtual bool restoreState();

	Element *elementById(const Id &id);

	Element *mElement;
	EditorViewScene const *mScene;
	Id mId;
	bool mSceneWasRemoved;

private slots:
	void onSceneWasRemoved();
};

}
}
