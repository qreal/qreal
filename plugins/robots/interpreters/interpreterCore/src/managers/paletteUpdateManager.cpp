#include "interpreterCore/managers/paletteUpdateManager.h"

using namespace interpreterCore;
using namespace interpreterBase;

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
