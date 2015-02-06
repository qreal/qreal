#include "elementCommand.h"

using namespace qReal::commands;

ElementCommand::ElementCommand(EditorViewScene const *scene, const Id &id)
	: mElement(nullptr), mScene(scene), mId(id), mSceneWasRemoved(false)
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

void ElementCommand::setId(const Id &id)
{
	mId = id;
	reinitElement();
}

bool ElementCommand::reinitElement()
{
	mElement = elementById(mId);
	return mElement != nullptr;
}

Element *ElementCommand::elementById(const Id &id)
{
	return mScene ? mScene->getElem(id) : nullptr;
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
