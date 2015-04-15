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

#include <qrgui/plugins/toolPluginInterface/customizer.h>

namespace ubiq {
namespace generator {

/// Customizes QReal interface to show Ubiq-specific details.
class Customizer : public qReal::Customizer
{
public:
	/// Window title shall reflect Technology name.
	virtual QString windowTitle() const
	{
		return QObject::tr("QReal:Ubiq");
	}

	/// Connections are extensively used by this plugin, so they are enabled.
	virtual bool showConnectionRelatedMenus() const
	{
		return true;
	}
};

}
}
