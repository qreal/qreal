#pragma once

#include <qrgui/mainwindow/mainWindowInterpretersInterface.h>

#include "kitPluginManager.h"

namespace interpreterCore {

class PaletteUpdateManager : public QObject
{
	Q_OBJECT

public:
	PaletteUpdateManager(qReal::gui::MainWindowInterpretersInterface &paletteProvider);

public slots:
	void updatePalette();

private:
	qReal::gui::MainWindowInterpretersInterface &mPaletteProvider;
};

}
