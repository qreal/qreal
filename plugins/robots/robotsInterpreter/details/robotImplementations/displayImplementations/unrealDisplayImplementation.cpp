#include "unrealDisplayImplementation.h"
#include "../details/nxtDisplay.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::displayImplementations;
using namespace graphicsUtils;

UnrealDisplayImplementation::UnrealDisplayImplementation(d2Model::D2RobotModel *d2Model)
	: mD2Model(d2Model)
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

void UnrealDisplayImplementation::clearScreen()
{
	mCurrentImage = QImage();
	mD2Model->display()->repaintDisplay();
}

void UnrealDisplayImplementation::paint(QPainter *painter)
{
	painter->drawImage(QRect(QPoint(), mD2Model->display()->displaySize()), mCurrentImage);
}
