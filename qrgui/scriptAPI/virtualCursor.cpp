#include "virtualCursor.h"

using namespace qReal;
using namespace gui;

VirtualCursor::VirtualCursor(MainWindow *mainWindow)
	: QLabel(mainWindow)
	, mMainWindow(mainWindow)
	, mIsPaletteElementAttached(false)
{
	setFixedSize(64, 64);
	show();
	setWindowFlags(Qt::WindowStaysOnTopHint);
	raise();

	update();
	//attachPaletteElement();
}

void VirtualCursor::attachPaletteElement(QString name)
{
	mPaletteElement = name;
	update();
}

void VirtualCursor::dropPaletteElement(QString name)
{
	//TO DO: detach icon, made block
}

void VirtualCursor::paintEvent(QPaintEvent *event)
{
	QPainter mPainter(this);
	QPixmap const virtCursorIcon(":/icons/virtcursor.png");
	QPixmap const pix(":/icons/run.png");
	mPainter.drawPixmap(0,0, virtCursorIcon	);
	mPainter.drawPixmap(32,32,pix);
}
