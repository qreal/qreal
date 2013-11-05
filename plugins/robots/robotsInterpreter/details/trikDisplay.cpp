#include "trikDisplay.h"
#include "ui_trikDisplay.h"

using namespace qReal::interpreters::robots::details;

TrikDisplay::TrikDisplay(QWidget *parent)
	: QLabel(parent)
	, mUi(new Ui::TrikDisplay)
{
	mUi->setupUi(this);
}

TrikDisplay::~TrikDisplay()
{
	delete mUi;
}

void TrikDisplay::setPainter(graphicsUtils::PainterInterface *painter)
{
	mUi->display->setPainter(painter);
}

QSize TrikDisplay::displaySize() const
{
	return mUi->display->size();
}

void TrikDisplay::repaintDisplay()
{
	mUi->display->repaint();
}

bool TrikDisplay::leftButtonIsDown() const
{
	return mUi->buttonLeft->isDown();
}

bool TrikDisplay::rightButtonIsDown() const
{
	return mUi->buttonRight->isDown();
}

bool TrikDisplay::upButtonIsDown() const
{
	return mUi->buttonUp->isDown();
}

bool TrikDisplay::downButtonIsDown() const
{
	return mUi->buttonDown->isDown();
}

bool TrikDisplay::downLeftButtonIsDown() const
{
	return mUi->buttonDownLeft->isDown();
}

bool TrikDisplay::attachButtonIsDown() const
{
	return mUi->buttonAttach->isDown();
}

bool TrikDisplay::onButtonIsDown() const
{
	return mUi->buttonOn->isDown();
}
