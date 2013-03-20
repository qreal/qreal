#include "elementCommand.h"

using namespace qReal::commands;

ElementCommand::ElementCommand(EditorViewScene const *scene, Id const &id)
	: mElement(NULL), mScene(scene), mId(id)
{
	reinitElement();
}

EditorViewScene const *ElementCommand::scene() const
{
	return mScene;
}

Id ElementCommand::elementId() const
{
	return mId;
}

void ElementCommand::setScene(EditorViewScene const *scene)
{
	mScene = scene;
	reinitElement();
}

void ElementCommand::setId(Id const &id)
{
	mId = id;
	reinitElement();
}

bool ElementCommand::reinitElement()
{
	if (mScene) {
		mElement = mScene->getElem(mId);
	}
	return mElement != NULL;
}

bool ElementCommand::restoreState()
{
	return reinitElement();
}

bool ElementCommand::execute()
{
	return reinitElement();
}
