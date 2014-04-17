#include "hintReporter.h"

using namespace qReal;
using namespace gui;

HintReporter::HintReporter(MainWindow *mainWindow)
	: QLabel(mainWindow)
	, mMainWindow(mainWindow)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setWindowFlags(Qt::WindowStaysOnTopHint);
	//setAttribute(Qt::WA_TranslucentBackground);
	setAutoFillBackground(false);
	setStyleSheet("background-color: #ffffff; border: 1px solid; border-radius: 7px; border-color: #303030;"); // по вкусу
	setFixedSize(160,160);
	move(mMainWindow->width() -50, mMainWindow->height() -50);
	show();
	raise();
}

void HintReporter::addHint(QString const &hint)
{
	setText(hint);
}
