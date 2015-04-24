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

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include "blocksFactoryManager.h"

namespace interpreterCore {

/// Updates palette when selected robot model is changed: hides blocks from other kits and disables blocks from current
/// kit that are not supported by selected model.
class PaletteUpdateManager : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param paletteProvider - contains methods for working with the palette.
	/// @param factoryManager - provides information about currently enabled blocks.
	/// @param parent - parent of this object in terms of Qt memory management system.
	PaletteUpdateManager(qReal::gui::MainWindowInterpretersInterface &paletteProvider
			, const BlocksFactoryManagerInterface &factoryManager
			, QObject *parent = 0);

public slots:
	/// Called when selected robot model is changed, updates palette.
	void updatePalette(kitBase::robotModel::RobotModelInterface &currentModel);

private:
	qReal::gui::MainWindowInterpretersInterface &mPaletteProvider;
	const BlocksFactoryManagerInterface &mFactoryManager;
};

}
