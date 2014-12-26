#pragma once

#include <qrgui/mainWindow/mainWindowInterpretersInterface.h>
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
			, BlocksFactoryManagerInterface const &factoryManager
			, QObject *parent = 0);

public slots:
	/// Called when selected robot model is changed, updates palette.
	void updatePalette(interpreterBase::robotModel::RobotModelInterface &currentModel);

private:
	qReal::gui::MainWindowInterpretersInterface &mPaletteProvider;
	BlocksFactoryManagerInterface const &mFactoryManager;
};

}
