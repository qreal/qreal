#pragma once

#include <QtWidgets/QLabel>
#include <QtCore/QString>

#include "mainwindow/mainWindow.h"

namespace qReal {
namespace gui {

class VirtualCursor : public QLabel
{
	Q_OBJECT

public:
	explicit VirtualCursor(MainWindow *mainWindow);

	void attachPaletteElement(QString name);
	void dropPaletteElement(QString name);

protected:
	void paintEvent(QPaintEvent *event);

private:
	MainWindow *mMainWindow;
	bool mIsPaletteElementAttached;
	QString mPaletteElement;
};

}
}
