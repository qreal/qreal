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

#include "interpreterCore/managers/paletteUpdateManager.h"

using namespace interpreterCore;
using namespace kitBase;

PaletteUpdateManager::PaletteUpdateManager(qReal::gui::MainWindowInterpretersInterface &paletteProvider
		, const BlocksFactoryManagerInterface &factoryManager, QObject *parent)
	: QObject(parent)
	, mPaletteProvider(paletteProvider)
	, mFactoryManager(factoryManager)
{
}

void PaletteUpdateManager::updatePalette(robotModel::RobotModelInterface &currentModel)
{
	mPaletteProvider.beginPaletteModification();
	mPaletteProvider.setEnabledForAllElementsInPalette(false);
	mPaletteProvider.setVisibleForAllElementsInPalette(false);

	for (const qReal::Id &id : mFactoryManager.visibleBlocks(currentModel)) {
		mPaletteProvider.setElementInPaletteVisible(id, true);
	}

	for (const qReal::Id &id : mFactoryManager.enabledBlocks(currentModel)) {
		mPaletteProvider.setElementInPaletteEnabled(id, true);
	}

	mPaletteProvider.endPaletteModification();
}
