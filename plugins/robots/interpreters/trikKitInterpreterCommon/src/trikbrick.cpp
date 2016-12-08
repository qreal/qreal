#include <trikKitInterpreterCommon/trikbrick.h>

#include <trikKit/robotModel/parts/trikShell.h>
#include <trikKit/robotModel/parts/trikLineSensor.h>
#include <kitBase/robotModel/robotParts/gyroscopeSensor.h>
#include <kitBase/robotModel/robotParts/encoderSensor.h>
#include <kitBase/robotModel/robotModelUtils.h>
///todo: temporary
#include <trikKitInterpreterCommon/robotModel/twoD/parts/twoDDisplay.h>

#include <QtCore/QTimer>
using namespace trik;

static const int updateInterval = 25;
TrikBrick::TrikBrick(const QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> &model)
	: mTwoDRobotModel(model), mDisplay(model), mKeys(model)
{
	connect(this, &TrikBrick::log, this, &TrikBrick::printToShell);
	mSensorUpdater.setInterval(updateInterval);
	connect(&mSensorUpdater, &QTimer::timeout, [model](){
		model->updateSensorsValues(); /// @todo: maybe connect to model directly?
	});
//	QTimer *t = new QTimer(this);
//	t->setInterval(25);
//	connect(t, &QTimer::timeout, [this](){
//		qDebug("here");
//		for (const auto &e : mEncoders) {
//			e->read();
//		}
//		for (const auto &s : mSensors) {
//			s->read();
//		}
//	});
}

TrikBrick::~TrikBrick()
{
	qDeleteAll(mMotors);
	qDeleteAll(mSensors);
	qDeleteAll(mEncoders);
	qDeleteAll(mLineSensors);
}

void TrikBrick::reset()
{
	mKeys.reset();///@todo: reset motos/device maps?
	//mDisplay.reset(); - is actually needed? Crashes app at exit
	for (const auto &m : mMotors) {
		m->powerOff();
	}
	for (const auto &e : mEncoders) {
		e->reset();
	}
	QMetaObject::invokeMethod(&mSensorUpdater, "stop"); // failproof against timer manipulation in another thread
	//mSensorUpdater.stop(); /// maybe needed in other places too.
}

void TrikBrick::printToShell(const QString &msg)
{
	using namespace kitBase::robotModel;
	robotParts::Shell* sh =
	        RobotModelUtils::findDevice<robotParts::Shell>(*mTwoDRobotModel, "ShellPort");
	if (sh == nullptr) {
		qDebug("Error: 2d model shell part was not found");
		return;
	}
	sh->print(msg);
}

void TrikBrick::init()
{
	mDisplay.init();
//	for (const auto &m : mMotors) {
//		m->powerOff();
//	}
//	for (const auto &e : mEncoders) {
//		e->read();
//	}
//	for (const auto &s : mSensors) {
//		s->read();
//	}
	mTwoDRobotModel->updateSensorsValues();
	mMotors.clear(); // needed? reset?
	mSensors.clear();
	mEncoders.clear();
	mKeys.init();
	QMetaObject::invokeMethod(&mSensorUpdater, "start"); // failproof against timer manipulation in another thread
	//mSensorUpdater.start();
}

void TrikBrick::stop() {
	/// @todo: properly implement this?
	mTwoDRobotModel->stopRobot();
//	for (const auto &m : mMotors) {
//		m->powerOff();
//	}
//	for (const auto &e : mEncoders) {
//		e->reset();
//	}
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

QStringList TrikBrick::encoderPorts() const {
	return mEncoders.keys();
}

trikControl::VectorSensorInterface *TrikBrick::accelerometer() {
	using namespace kitBase::robotModel;
	if (mAccelerometer.isNull()) {
		auto a = RobotModelUtils::findDevice<robotParts::AccelerometerSensor>(*mTwoDRobotModel
																			  , "AccelerometerPort");
		if (a == nullptr) {
			emit error(tr("No configured accelerometer"));
			return nullptr;
		}
		mAccelerometer.reset(new TrikAccelerometerAdapter(a));
	}
	return mAccelerometer.data();
}

trikControl::VectorSensorInterface *TrikBrick::gyroscope() {
	using namespace kitBase::robotModel;
	if (mGyroscope.isNull()) {
		auto a = RobotModelUtils::findDevice<robotParts::GyroscopeSensor>(*mTwoDRobotModel
																		  , "GyroscopePort");
		if (a == nullptr) {
			emit error(tr("No configured gyroscope"));
			return nullptr;
		}
		mGyroscope.reset(new TrikGyroscopeAdapter(a->port(), mTwoDRobotModel->engine()));
	}
	return mGyroscope.data();
}

trikControl::LineSensorInterface *TrikBrick::lineSensor(const QString &port) {
	using namespace trik::robotModel::parts;
	using namespace kitBase::robotModel;
	if (port == "video0") {
		return lineSensor("LineSensorPort"); // seems to be the case for 2d model
	}
	if (!mLineSensors.contains(port)) {
		TrikLineSensor * sens =
				RobotModelUtils::findDevice<TrikLineSensor>(*mTwoDRobotModel, port);
		if (sens == nullptr) {
			emit error(tr("No configured LineSensor on port: ") + port);
			return nullptr;
		}
		mLineSensors[port] = new TrikLineSensorAdapter(sens);
	}
	return mLineSensors[port];
}

trikControl::EncoderInterface *TrikBrick::encoder(const QString &port) {
	using namespace kitBase::robotModel;
	if (!mEncoders.contains(port)) {
		robotParts::EncoderSensor * enc =
		        RobotModelUtils::findDevice<robotParts::EncoderSensor>(*mTwoDRobotModel, port);
		if (enc == nullptr) {
			emit error(tr("No configured encoder on port: ") + port);
			return nullptr;
		}
		mEncoders[port] = new TrikEncoderAdapter(enc->port(), mTwoDRobotModel->engine());
	}
	return mEncoders[port];
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

trikControl::LedInterface *TrikBrick::led() {
	using namespace trik::robotModel::parts;
	using namespace kitBase::robotModel;
	if (mLed.isNull()) {
		auto l = RobotModelUtils::findDevice<TrikLed>(*mTwoDRobotModel, "LedPort");
		if (l == nullptr) {
			emit error(tr("No configured led"));
			return nullptr;
		}
		mLed.reset(new TrikLedAdapter(l));
	}
	return mLed.data();
}

