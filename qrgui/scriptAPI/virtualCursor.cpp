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
}

void VirtualCursor::attachPaletteElement(QIcon icon)
{
	mIsPaletteElementAttached = true;
	mPaletteElementIcon = icon;
	update();
}

void VirtualCursor::paintEvent(QPaintEvent *event)
{
	QPainter mPainter(this);
	QPixmap const virtCursorIcon(":/icons/virtcursor.png");
	mPainter.drawPixmap(0,0, virtCursorIcon	);
	if (mIsPaletteElementAttached) {
		mPainter.drawPixmap(QPoint(32,32), mPaletteElementIcon.pixmap(32, 32));
	}
}

void VirtualCursor::detachPaletteElementIcon()
{
	 mIsPaletteElementAttached = false;
	 update();
}
