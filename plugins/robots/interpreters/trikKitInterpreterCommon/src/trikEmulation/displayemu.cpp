#include <trikKitInterpreterCommon/trikEmulation/displayemu.h>

#include <kitBase/robotModel/robotModelUtils.h>
///todo:
#include <trikKitInterpreterCommon/robotModel/twoD/parts/twoDDisplay.h>

trik::TrikDisplayEmu::TrikDisplayEmu(QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> model) : mTwoDRobotModel(model)
{
	isSmiling = false;
}

trikControl::DisplayWidgetInterface &trik::TrikDisplayEmu::graphicsWidget() {
	trikControl::DisplayWidgetInterface temp;
	return temp; // bad
}

void trik::TrikDisplayEmu::showImage(const QString &fileName)
{
		trik::robotModel::parts::TrikDisplay * const display =
				kitBase::robotModel::RobotModelUtils::findDevice<trik::robotModel::parts::TrikDisplay>(*mTwoDRobotModel, "DisplayPort");
		if (display) {
			bool res = QMetaObject::invokeMethod(display,
			"drawSmile",
			Q_ARG(bool, isSmiling));
			//display->drawSmile(false);
		}
		isSmiling = !isSmiling;
}
