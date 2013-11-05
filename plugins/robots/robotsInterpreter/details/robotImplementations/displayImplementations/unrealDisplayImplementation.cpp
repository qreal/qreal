#include "unrealDisplayImplementation.h"
#include "../details/nxtDisplay.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::displayImplementations;
using namespace graphicsUtils;

UnrealDisplayImplementation::UnrealDisplayImplementation(d2Model::D2RobotModel *d2Model)
	: mD2Model(d2Model)
	, mBackground(Qt::transparent)
{
}

void UnrealDisplayImplementation::attachToPaintWidget()
{
	mD2Model->display()->setPainter(this);
}

void UnrealDisplayImplementation::read()
{
//	emit response(mD2Model->display()->leftButtonIsDown()
//		, mD2Model->display()->rightButtonIsDown()
//		, mD2Model->display()->centralButtonIsDown()
//		, mD2Model->display()->bottomButtonIsDown());
}

void UnrealDisplayImplementation::drawImage(QImage const &image)
{
	mCurrentImage = image;
	mD2Model->display()->repaintDisplay();
}

void UnrealDisplayImplementation::setBackground(QColor const &color)
{
	mBackground = color;
	mD2Model->display()->repaintDisplay();
}

void UnrealDisplayImplementation::clearScreen()
{
	mCurrentImage = QImage();
	mBackground = Qt::transparent;
	mD2Model->display()->repaintDisplay();
}

void UnrealDisplayImplementation::paint(QPainter *painter)
{
	QRect const displayRect(QPoint(), mD2Model->display()->displaySize());
	painter->save();
	painter->setPen(mBackground);
	painter->setBrush(mBackground);
	painter->drawRect(displayRect);
	painter->drawImage(displayRect, mCurrentImage);
	painter->restore();
}
