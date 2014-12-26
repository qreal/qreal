#include "elementCommand.h"

using namespace qReal::commands;

ElementCommand::ElementCommand(EditorViewScene const *scene, Id const &id)
	: mElement(NULL), mScene(scene), mId(id), mSceneWasRemoved(false)
{
	reinitElement();
	connect(mScene, SIGNAL(destroyed()), SLOT(onSceneWasRemoved()));
}

ElementCommand::~ElementCommand()
{
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
	mElement = elementById(mId);
	return mElement != NULL;
}

Element *ElementCommand::elementById(Id const &id)
{
	return mScene ? mScene->getElem(id) : NULL;
}

void ElementCommand::onSceneWasRemoved()
{
	mSceneWasRemoved = true;
}

bool ElementCommand::execute()
{
	return !mSceneWasRemoved && reinitElement();
}

bool ElementCommand::restoreState()
{
	return !mSceneWasRemoved && reinitElement();
}
