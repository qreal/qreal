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

#include "controller/commands/abstractCommand.h"
#include "editor/editorView.h"

namespace qReal {
namespace gui {
namespace editor {
namespace commands {

/// A base for all commands using concrete element instance on some scene
class ElementCommand : public qReal::commands::AbstractCommand
{
	Q_OBJECT

public:
	ElementCommand(const EditorViewScene *scene, const Id &id);
	virtual ~ElementCommand();

	const EditorViewScene *scene() const;
	Id elementId() const;
	void setScene(const EditorViewScene *scene);
	void setId(const Id &id);

protected:
	/// Called when our element instance possibly has changed
	virtual bool reinitElement();

	virtual bool execute();
	virtual bool restoreState();

	Element *elementById(const Id &id);

	Element *mElement;
	const EditorViewScene *mScene;
	Id mId;
	bool mSceneWasRemoved;

private slots:
	void onSceneWasRemoved();
};

}
}
}
}
