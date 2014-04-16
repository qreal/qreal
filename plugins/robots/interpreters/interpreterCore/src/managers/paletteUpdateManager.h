#pragma once

#include <qrgui/mainwindow/mainWindowInterpretersInterface.h>
#include "blocksFactoryManager.h"

namespace interpreterCore {

class PaletteUpdateManager : public QObject
{
	Q_OBJECT

public:
	PaletteUpdateManager(qReal::gui::MainWindowInterpretersInterface &paletteProvider
			, BlocksFactoryManager const &factoryManager
			, QObject *parent = 0);

public slots:
	void updatePalette(interpreterBase::robotModel::RobotModelInterface &currentModel);

private:
	qReal::gui::MainWindowInterpretersInterface &mPaletteProvider;
	BlocksFactoryManager const &mFactoryManager;
};

}
