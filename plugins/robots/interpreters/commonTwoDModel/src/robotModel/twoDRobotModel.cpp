#include "commonTwoDModel/robotModel/twoDRobotModel.h"

#include <qrkernel/exception/exception.h>

#include "commonTwoDModel/robotModel/parts/button.h"
#include "commonTwoDModel/robotModel/parts/colorSensorFull.h"
#include "commonTwoDModel/robotModel/parts/colorSensorPassive.h"
#include "commonTwoDModel/robotModel/parts/colorSensorRed.h"
#include "commonTwoDModel/robotModel/parts/colorSensorGreen.h"
#include "commonTwoDModel/robotModel/parts/colorSensorBlue.h"
#include "commonTwoDModel/robotModel/parts/display.h"
#include "commonTwoDModel/robotModel/parts/encoderSensor.h"
#include "commonTwoDModel/robotModel/parts/lightSensor.h"
#include "commonTwoDModel/robotModel/parts/motor.h"
#include "commonTwoDModel/robotModel/parts/rangeSensor.h"
#include "commonTwoDModel/robotModel/parts/touchSensor.h"
#include "commonTwoDModel/robotModel/parts/gyroscope.h"
#include "commonTwoDModel/robotModel/parts/accelerometer.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

#include <QtCore/QDebug>

using namespace twoDModel::robotModel;
using namespace interpreterBase::robotModel;

TwoDRobotModel::TwoDRobotModel(RobotModelInterface &realModel)
	: CommonRobotModel(realModel.kitId(), realModel.robotId())
	, mRealModel(&realModel)
	, mEngine(nullptr)
{
	for (PortInfo const &port : realModel.availablePorts()) {
		addAllowedConnection(port, realModel.allowedDevices(port));
	}
}

TwoDRobotModel::TwoDRobotModel(QString const &robotId)
	:CommonRobotModel("", robotId)
	, mRealModel(nullptr)
	, mEngine(nullptr)
{
}

QString TwoDRobotModel::name() const
{
	return "TwoDRobotModelFor" + mRealModel->name();
}

QString TwoDRobotModel::friendlyName() const
{
	QRegExp versionRegExp("\\(.*\\)");
	int const pos = versionRegExp.indexIn(mRealModel->friendlyName());
	if (pos == -1) {
		return tr("2D Model");
	}

	return tr("2D Model") + " " + versionRegExp.capturedTexts().at(0);
}

bool TwoDRobotModel::needsConnection() const
{
	return false;
}

utils::TimelineInterface &TwoDRobotModel::timeline()
{
	return mEngine->modelTimeline();
}

QList<PortInfo> TwoDRobotModel::configurablePorts() const
{
	return mRealModel->configurablePorts();
}

QList<DeviceInfo> TwoDRobotModel::convertibleBases() const
{
	return mRealModel->convertibleBases();
}

twoDModel::engine::TwoDModelEngineInterface *TwoDRobotModel::engine()
{
	return mEngine;
}

void TwoDRobotModel::setEngine(engine::TwoDModelEngineInterface &engine)
{
	mEngine = &engine;
}

robotParts::Device *TwoDRobotModel::createDevice(PortInfo const &port, DeviceInfo const &deviceInfo)
{
	if (deviceInfo.isA<robotParts::Button>()) {
		return new parts::Button(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::Motor>()) {
		return new parts::Motor(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::EncoderSensor>()) {
		return new parts::EncoderSensor(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::TouchSensor>()) {
		return new parts::TouchSensor(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::RangeSensor>()) {
		return new parts::RangeSensor(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::LightSensor>()) {
		return new parts::LightSensor(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotModel::parts::ColorSensorPassive>()) {
		return new parts::ColorSensorPassive(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotModel::parts::ColorSensorFull>()) {
		return new parts::ColorSensorFull(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotModel::parts::ColorSensorRed>()) {
		return new parts::ColorSensorRed(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotModel::parts::ColorSensorGreen>()) {
		return new parts::ColorSensorGreen(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotModel::parts::ColorSensorBlue>()) {
		return new parts::ColorSensorBlue(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotModel::parts::Gyroscope>()) {
		return new parts::Gyroscope(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotModel::parts::Accelerometer>()) {
		return new parts::Accelerometer(deviceInfo, port, *mEngine);
	}

	qDebug() << "Unknown device " + deviceInfo.toString() + " requested on port " + port.name();
	//	throw qReal::Exception("Unknown device " + deviceInfo.toString() + " requested on port " + port.name());
	return nullptr;
}
