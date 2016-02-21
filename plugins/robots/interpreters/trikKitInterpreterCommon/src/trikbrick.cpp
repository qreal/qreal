#include <trikKitInterpreterCommon/trikbrick.h>

#include <kitBase/robotModel/robotModelUtils.h>
///todo: temporary
#include <trikKitInterpreterCommon/robotModel/twoD/parts/twoDDisplay.h>
using namespace trik;

TrikBrick::TrikBrick(const QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> &model) : mTwoDRobotModel(model), mDisplay(model)
{
}

void TrikBrick::init()
{
	mDisplay.init();
}

trikControl::DisplayInterface *TrikBrick::display() {
//	trik::robotModel::parts::TrikDisplay * const display =
//			kitBase::robotModel::RobotModelUtils::findDevice<trik::robotModel::parts::TrikDisplay>(*mTwoDRobotModel, "DisplayPort");
//	if (display) {
//		bool res = QMetaObject::invokeMethod(display,
//		"drawSmile",
//		Qt::QueuedConnection, // connection type, auto?
//		Q_ARG(bool, false));
//		//display->drawSmile(false);
//		printf(res ? "true" : "false");
//	}
//	return nullptr;
	return &mDisplay;
}
