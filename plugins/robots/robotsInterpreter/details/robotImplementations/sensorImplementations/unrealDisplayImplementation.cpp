#include "unrealDisplayImplementation.h"
#include "../details/nxtDisplay.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealDisplayImplementation::UnrealDisplayImplementation(d2Model::D2RobotModel *d2Model)
	: mD2Model(d2Model)
{
}

void UnrealDisplayImplementation::read()
{
	emit response(/*mD2Model->leftButtonIsDown()
				  /*,mDisplay->rightButtonIsDown()
				  ,mDisplay->centralButtonIsDown()
				  ,mDisplay->bottomButtonIsDown()*/true,true,true,true);
}
