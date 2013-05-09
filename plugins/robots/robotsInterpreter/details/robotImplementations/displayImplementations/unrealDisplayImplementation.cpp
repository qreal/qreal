#include "unrealDisplayImplementation.h"
#include "../details/nxtDisplay.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::displayImplementations;

UnrealDisplayImplementation::UnrealDisplayImplementation(d2Model::D2RobotModel *d2Model)
	: mD2Model(d2Model)
{
}

void UnrealDisplayImplementation::read()
{
	emit response(mD2Model->display()->leftButtonIsDown()
				  ,mD2Model->display()->rightButtonIsDown()
				  ,mD2Model->display()->centralButtonIsDown()
				  ,mD2Model->display()->bottomButtonIsDown());
}

void UnrealDisplayImplementation::drawPixel(int x, int y)
{

}

void UnrealDisplayImplementation::drawLine(int x1, int y1, int x2, int y2)
{

}

void UnrealDisplayImplementation::drawCircle(int x, int y, int radius)
{

}

void UnrealDisplayImplementation::printText(int x, int y, QString text)

{

}
