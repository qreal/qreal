#include "paletteUpdateManager.h"

using namespace interpreterCore;
using namespace interpreterBase;

PaletteUpdateManager::PaletteUpdateManager(qReal::gui::MainWindowInterpretersInterface &paletteProvider
		, BlocksFactoryManager const &factoryManager, QObject *parent)
	: QObject(parent)
	, mPaletteProvider(paletteProvider)
	, mFactoryManager(factoryManager)
{
}

void PaletteUpdateManager::updatePalette(robotModel::RobotModelInterface &currentModel)
{
	QSet<qReal::Id> enabledBlocks;

	for (blocksBase::BlocksFactoryInterface const *factory : mFactoryManager.factoriesFor(currentModel)) {
		enabledBlocks += factory->providedBlocks().toSet();
		enabledBlocks -= factory->blocksToDisable().toSet();
	}

	mPaletteProvider.setEnabledForAllElementsInPalette(false);
	for (qReal::Id const &id : enabledBlocks) {
		mPaletteProvider.setElementInPaletteEnabled(id, true);
	}
}
