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

#include "systemFacade.h"

#include <qrgui/plugins/pluginManager/editorManager.h>

using namespace qReal;

SystemFacade::SystemFacade()
	: mEditorManager(new EditorManager)
	, mModels(QString(), mEditorManager)
{
	QObject::connect(&mModels.logicalModelAssistApi(), &models::LogicalModelAssistApi::elementAdded
			, &mEvents, &SystemEvents::logicalElementAdded);
	QObject::connect(&mModels.graphicalModelAssistApi(), &models::GraphicalModelAssistApi::elementAdded
			, &mEvents, &SystemEvents::graphicalElementAdded);
}

EditorManagerInterface &SystemFacade::editorManager()
{
	return mEditorManager;
}

models::Models &SystemFacade::models()
{
	return mModels;
}

SystemEvents &SystemFacade::events()
{
	return mEvents;
}
