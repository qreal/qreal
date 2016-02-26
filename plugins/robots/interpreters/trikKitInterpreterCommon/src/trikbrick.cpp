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
	mMotors.clear(); // needed? reset?
	mSensors.clear();
}

trikControl::MotorInterface *TrikBrick::motor(const QString &port)
{
	using namespace kitBase::robotModel;
	if (!mMotors.contains(port)) {
		robotParts::Motor * mot =
		        RobotModelUtils::findDevice<robotParts::Motor>(*mTwoDRobotModel, port);
		if (mot == nullptr) {
			emit error(tr("No configured motor on port: ") + port);
			return nullptr;
		}
		mMotors[port] = new TrikMotorEmu(mot);
	}
	return mMotors[port];
}

trikControl::SensorInterface *TrikBrick::sensor(const QString &port)
{
	//testing
	using namespace kitBase::robotModel;
	if (!mSensors.contains(port)) {
		robotParts::ScalarSensor * sens =
		        RobotModelUtils::findDevice<robotParts::ScalarSensor>(*mTwoDRobotModel, port);
		if (sens == nullptr) {
			emit error(tr("No configured sensor on port: ") + port);
			return nullptr;
		}
		mSensors[port] = new TrikSensorEmu(sens);
	}
	return mSensors[port];
}

QStringList TrikBrick::motorPorts(trikControl::MotorInterface::Type type) const
{
//	QLOG_INFO() << "Motor type is ignored";
	return mMotors.keys();
}

QStringList TrikBrick::sensorPorts(trikControl::SensorInterface::Type type) const
{
//	QLOG_INFO() << "Sensor type is ignored";
	return mMotors.keys();
}

trikControl::DisplayInterface *TrikBrick::display()
{
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

