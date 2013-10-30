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
