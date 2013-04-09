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
