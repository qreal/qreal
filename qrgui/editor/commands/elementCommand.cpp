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

#include "elementCommand.h"

using namespace qReal;
using namespace qReal::gui::editor;
using namespace qReal::gui::editor::commands;

ElementCommand::ElementCommand(const EditorViewScene *scene, const Id &id)
	: mElement(nullptr), mScene(scene), mId(id), mSceneWasRemoved(false)
{
	reinitElement();
	connect(mScene, SIGNAL(destroyed()), SLOT(onSceneWasRemoved()));
}

ElementCommand::~ElementCommand()
{
}

const EditorViewScene *ElementCommand::scene() const
{
	return mScene;
}

Id ElementCommand::elementId() const
{
	return mId;
}

void ElementCommand::setScene(const EditorViewScene *scene)
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
