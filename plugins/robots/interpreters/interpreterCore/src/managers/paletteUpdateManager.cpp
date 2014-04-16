#include "paletteUpdateManager.h"

#include <qrkernel/settingsManager.h>

using namespace interpreterCore;

PaletteUpdateManager::PaletteUpdateManager(qReal::gui::MainWindowInterpretersInterface &paletteProvider)
	: mPaletteProvider(paletteProvider)
{
}

void PaletteUpdateManager::updatePalette()
{
//	QString const selectedKitId = qReal::SettingsManager::value("SelectedRobotKit").toString();
//	if (mKitPluginManager.kitIds().isEmpty() || !mKitPluginManager.kitIds().contains(selectedKitId)) {
//		mPaletteProvider.setEnabledForAllElementsInPalette(false);
//		return;
//	}

//	interpreterBase::KitPluginInterface const selectedKit = mKitPluginManager.kitById(kitId);
//	qReal::IdList const toEnable = selectedKit.specificBlocks();
//	qReal::IdList const toDisable = selectedKit.unsupportedBlocks();

}
