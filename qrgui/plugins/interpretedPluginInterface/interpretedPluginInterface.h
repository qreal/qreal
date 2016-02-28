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

#include <plugins/toolPluginInterface/pluginInterface.h>
#include <qrgui/plugins/toolPluginInterface/actionInfo.h>

namespace qReal {

/// Base class for interpreted plugins.
class InterpretedPluginInterface : public PluginInterface
{
public:
	/// Shall be overriden in concrete plugin to initialize itself.
	/// @param configurator - Object that contains all needed information for a plugin, like refs to models.
	/// @param metamodelRepoApi - Object that contains information about metamodel
	virtual void init(
			const PluginConfigurator &configurator
			, qrRepo::LogicalRepoApi &metamodelRepoApi
			, EditorManagerInterface *editorManagerInterface
			) = 0;

	/// Shall be overriden in concrete plugin; returns list of additional actions for elements in palette.
	virtual QList<QAction *> menuActionList() const
	{
		return {};
	}
};

}

Q_DECLARE_INTERFACE(qReal::InterpretedPluginInterface, "ru.spbsu.QReal.InterpretedPluginInterface/0.2")
