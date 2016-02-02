#include <interpreterCore/interpreter/trikbrick.h>

#include <kitBase/robotModel/robotModelUtils.h>
///todo: temporaly
#include <plugins/robots/interpreters/trikKitInterpreterCommon/include/trikKitInterpreterCommon/robotModel/twoD/parts/twoDDisplay.h>

TrikBrick::TrikBrick(kitBase::robotModel::RobotModelInterface *model)
{
	mTwoDModel = model;
}

trikControl::DisplayInterface *TrikBrick::display() {
//	trik::robotModel::parts::TrikDisplay * const display =
//			kitBase::robotModel::RobotModelUtils::findDevice<trik::robotModel::parts::TrikDisplay>(*mTwoDModel, "DisplayPort");
//	if (display) display->drawSmile(false);
}
