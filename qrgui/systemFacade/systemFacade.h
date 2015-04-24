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

#include <qrgui/plugins/pluginManager/proxyEditorManager.h>
#include <qrgui/plugins/toolPluginInterface/systemEvents.h>
#include <qrgui/models/models.h>

#include "qrgui/systemFacade/systemFacadeDeclSpec.h"

namespace qReal {

/// Initializes and provides all main backend parts of the visual modeling system.
/// This class is just a convenient way to use system infrastrucure from different frontends.
class QRGUI_SYSTEM_FACADE_EXPORT SystemFacade
{
public:
	SystemFacade();

	/// Returns a reference to an entity that obtains and manages the plugins with visual languages descriptions.
	EditorManagerInterface &editorManager();

	/// Returns a reference to models facade object.
	models::Models &models();

	/// Returns a reference to an object that can be used for
	/// subscription to events inside the visual modeling system engine.
	SystemEvents &events();

private:
	ProxyEditorManager mEditorManager;
	models::Models mModels;
	SystemEvents mEvents;
};

}
