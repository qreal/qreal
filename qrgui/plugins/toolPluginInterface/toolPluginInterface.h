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

#include "qrgui/plugins/toolPluginInterface/pluginInterface.h"

namespace qReal {

/// Base class for tool plugins, actually an interface with default implementations.
/// This is all that qrgui knows about tool plugins. Every tool plugin shall have
/// a class derived from this and override needed methods. Default implementations
/// are provided as "do nothing" behavior, so if a plugin doesn't need some features,
/// it shouldn't care about them.
class ToolPluginInterface : public PluginInterface
{
public:
	virtual ~ToolPluginInterface() {}

	/// Shall be overriden in concrete plugin to initialize itself.
	/// @param configurator Object that contains all needed information for a plugin, like refs to models.
	virtual void init(const PluginConfigurator &configurator)
	{
		Q_UNUSED(configurator);
	}
};

}

Q_DECLARE_INTERFACE(qReal::ToolPluginInterface, "ru.spbsu.QReal.ToolPluginInterface/0.2")
