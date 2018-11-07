/* Copyright 2016-2017 CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include <QtCore/QTimer>
#include <QtCore/QEventLoop>

#include <QtCore/QThread>

#include <trikKitInterpreterCommon/trikbrick.h>

#include <utils/abstractTimer.h>
#include <kitBase/robotModel/robotModelUtils.h>
#include <trikKit/robotModel/parts/trikShell.h>
#include <trikKit/robotModel/parts/trikLineSensor.h>
#include <kitBase/robotModel/robotParts/gyroscopeSensor.h>
#include <kitBase/robotModel/robotParts/encoderSensor.h>
#include <kitBase/robotModel/robotParts/random.h>
#include <kitBase/robotModel/robotParts/random.h>
#include <twoDModel/robotModel/parts/marker.h>
#include <twoDModel/engine/model/timeline.h>
#include <qrkernel/settingsManager.h>
#include <qrkernel/settingsListener.h>
#include <qrkernel/platformInfo.h>
#include <src/qtCameraImplementation.h>
#include <src/imitationCameraImplementation.h>

///todo: temporary
#include <trikKitInterpreterCommon/robotModel/twoD/parts/twoDDisplay.h>


using namespace trik;

TrikBrick::TrikBrick(const QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> &model)
	: mTwoDRobotModel(model)
	, mDisplay(model)
	, mKeys(model)
	, mSensorUpdater(model->timeline().produceTimer())
{
	connect(this, &TrikBrick::log, this, &TrikBrick::printToShell);
	mSensorUpdater->setRepeatable(true);
	mSensorUpdater->setInterval(model->updateIntervalForInterpretation()); // seems to be x2 of timeline tick
	connect(mSensorUpdater.data(), &utils::AbstractTimer::timeout, [model](){
		model->updateSensorsValues(); /// @todo: maybe connect to model directly?
	});

//	const QString path = qReal::SettingsManager::value("TrikSimulatedCameraImagesPath").toString();
	const QString path = "/home/greg/Documents/qreal/bin/debug"; // test only
	mImitationCamera.reset(new trikControl::ImitationCameraImplementation({"*.jpg","*.png"}, path));

	qReal::SettingsListener::listen("TrikSimulatedCameraImagesPath", [&](QString path) {
		mImitationCamera.reset(new trikControl::ImitationCameraImplementation({"*.jpg","*.png"}, path));
	}, this);

	qReal::SettingsListener::listen("TrikSimulatedCameraImagesFromProject", [&](bool checked) {
		if (checked) {
			const QString path = qReal::PlatformInfo::invariantSettingsPath("trikCameraImitationImagesDir");
			mImitationCamera.reset(new trikControl::ImitationCameraImplementation({"*.jpg","*.png"}, path));
		}
	}, this);
}

TrikBrick::~TrikBrick()
{
	qDeleteAll(mMotors);
	qDeleteAll(mSensors);
	qDeleteAll(mEncoders);
	qDeleteAll(mLineSensors);
	qDeleteAll(mTimers);
}

void TrikBrick::reset()
{
	mKeys.reset();///@todo: reset motos/device maps?
	//mDisplay.reset(); - is actually needed? Crashes app at exit
	emit stopWaiting();
	for (const auto &m : mMotors) {
		m->powerOff();
	}

	for (const auto &e : mEncoders) {
		e->reset();
	}

	for (const auto &t : mTimers) {
		t->stop();
	}

	qDeleteAll(mTimers);
	mTimers.clear();
	QMetaObject::invokeMethod(mSensorUpdater.data(), "stop"); // failproof against timer manipulation in another thread
}

void TrikBrick::printToShell(const QString &msg)
{
	using namespace kitBase::robotModel;
	using namespace trik::robotModel;
	parts::TrikShell* sh = RobotModelUtils::findDevice<parts::TrikShell>(*mTwoDRobotModel, "ShellPort");
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
	mGyroscope.reset(); // for some reason it won't reconnect to the robot parts otherwise.
	QMetaObject::invokeMethod(mSensorUpdater.data(), "start"); // failproof against timer manipulation in another thread
	//mSensorUpdater.start();
}

void TrikBrick::setCurrentDir(const QString &dir)
{
	mCurrentDir = QFileInfo(dir).dir(); // maybe can be constructed directly
}

void TrikBrick::setCurrentInputs(const QString &f)
{
	mIsExcerciseMode = true;
	if (f.isEmpty()) {
		return; // no inputs has been passed, no need to complain
	}
	QString file(f);
	QFile in(file);
	if (!in.open(QIODevice::ReadOnly | QIODevice::Text)) {
		emit warning(tr("Trying to read from file %1 failed").arg(file)); // todo: remove? It's only in exercise.
		//not really an error, usually
	}

	QStringList result;

	while (!in.atEnd()) {
		const auto line = in.readLine();
		result << QString::fromUtf8(line);
	}

	mInputs = result;
}

void TrikBrick::say(const QString &msg) {
	using namespace kitBase::robotModel;
	using namespace trik::robotModel;
	parts::TrikShell* sh = RobotModelUtils::findDevice<parts::TrikShell>(*mTwoDRobotModel, "ShellPort");
	if (sh == nullptr) {
		qDebug("Error: 2d model shell part was not found");
		return;
	}

	QMetaObject::invokeMethod(sh, "say", Q_ARG(const QString &, msg));
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
			emit error(tr("No configured motor on port: %1").arg(port));
			return nullptr;
		}
		mMotors[port] = new TrikMotorEmu(mot);
	}
	return mMotors[port];
}

trikControl::MarkerInterface *TrikBrick::marker()
{
	kitBase::robotModel::PortInfo markerPort = kitBase::robotModel::RobotModelUtils::findPort(*mTwoDRobotModel
			, "MarkerPort"
			, kitBase::robotModel::Direction::output);
	if (markerPort.isValid()) {
		using Marker = twoDModel::robotModel::parts::Marker;
		Marker* marker = kitBase::robotModel::RobotModelUtils::findDevice<Marker>(*mTwoDRobotModel, markerPort);
		mTrikProxyMarker.reset(new TrikProxyMarker(marker));
		return mTrikProxyMarker.data();
	}

	return nullptr;
}

trikControl::SensorInterface *TrikBrick::sensor(const QString &port)
{
	//testing
	using namespace kitBase::robotModel;
	if (!mSensors.contains(port)) {
		robotParts::ScalarSensor * sens =
				RobotModelUtils::findDevice<robotParts::ScalarSensor>(*mTwoDRobotModel, port);
		if (sens == nullptr) {
			emit error(tr("No configured sensor on port: %1").arg(port));
			return nullptr;
		}
		mSensors[port] = new TrikSensorEmu(sens);
	}
	return mSensors[port];
}

QStringList TrikBrick::motorPorts(trikControl::MotorInterface::Type type) const
{
	Q_UNUSED(type)
//	QLOG_INFO() << "Motor type is ignored";
	return mMotors.keys();
}

QStringList TrikBrick::sensorPorts(trikControl::SensorInterface::Type type) const
{
	Q_UNUSED(type)
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

trikControl::GyroSensorInterface *TrikBrick::gyroscope() {
	using namespace kitBase::robotModel;
	if (mGyroscope.isNull()) {
		auto a = RobotModelUtils::findDevice<robotParts::GyroscopeSensor>(*mTwoDRobotModel
				, "GyroscopePort");
		if (a == nullptr) {
			emit error(tr("No configured gyroscope"));
			return nullptr;
		}
		mGyroscope.reset(new TrikGyroscopeAdapter(a, mTwoDRobotModel));
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
			emit error(tr("No configured LineSensor on port: %1").arg(port));
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
			emit error(tr("No configured encoder on port: %1").arg(port));
			return nullptr;
		}

		mEncoders[port] = new TrikEncoderAdapter(enc->port(), mTwoDRobotModel->engine());
	}

	return mEncoders[port];
}

trikControl::DisplayInterface *TrikBrick::display()
{
//	trik::robotModel::parts::TrikDisplay * const display =
//			kitBase::robotModel::RobotModelUtils::findDevice<trik::robotModel::parts::TrikDisplay>(*mTwoDRobotModel
//					, "DisplayPort");
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

QVector<uint8_t> TrikBrick::getStillImage()
{
	const bool webCamera = qReal::SettingsManager::value("TrikWebCameraReal").toBool();

	if (webCamera) {
		const QString webCameraName = qReal::SettingsManager::value("TrikWebCameraRealName").toString();
		trikControl::QtCameraImplementation camera(webCameraName);
		camera.setTempDir(qReal::PlatformInfo::invariantSettingsPath("pathToTempFolder"));
		QVector<uint8_t> photo = camera.getPhoto();
		return photo;
	} else {
		QVector<uint8_t> photo = mImitationCamera->getPhoto();
		return photo;
	}
}

int TrikBrick::random(int from, int to)
{
	using namespace kitBase::robotModel;
	auto r = RobotModelUtils::findDevice<robotParts::Random>(*mTwoDRobotModel, "RandomPort");
	// maybe store it later, like the others
	if (!r) {
		emit error(tr("No cofigured random device"));
		return -1;
	}

	return r->random(from, to);
}

void TrikBrick::wait(int milliseconds)
{
	QEventLoop loop;
	auto &timeline = dynamic_cast<twoDModel::model::Timeline &> (mTwoDRobotModel->timeline());

	if (timeline.isStarted()) {
		QScopedPointer<utils::AbstractTimer> t(timeline.produceTimer());
		QTimer abortTimer;
		QMetaObject::Connection abortConnection;

		auto mainHandler = [this, &t, &loop, &timeline, &abortConnection]() {
			disconnect(abortConnection);
			disconnect(this, &TrikBrick::stopWaiting, nullptr, nullptr);
			disconnect(&timeline, &twoDModel::model::Timeline::beforeStop, nullptr, nullptr);
			disconnect(t.data(), &utils::AbstractTimer::timeout, nullptr, nullptr);
			loop.quit();
		};

		auto abortHandler = [mainHandler, &timeline]() {
			if (!timeline.isStarted()) {
				mainHandler();
			}
		};

		connect(t.data(), &utils::AbstractTimer::timeout, mainHandler);
		connect(this, &TrikBrick::stopWaiting, mainHandler);

		// timers that are produced by produceTimer() doesn't use stop singal
		// be careful, one who use just utils::AbstractTimer can stuck
		connect(&timeline, &twoDModel::model::Timeline::beforeStop, mainHandler);
		abortConnection = connect(&abortTimer, &QTimer::timeout, abortHandler);

		// because timer is depends on twoDModel::model::Timeline
		if (timeline.isStarted()) {
			t->start(milliseconds);
			abortTimer.start(10);
			loop.exec();
		} else {
			mainHandler();
		}
	}
}

quint64 TrikBrick::time() const
{
	return mTwoDRobotModel->timeline().timestamp();
}

QStringList TrikBrick::readAll(const QString &path)
{
	if (mIsExcerciseMode) {
		return mInputs;
	}
	//if (mCurrentDir) todo: check that the current working dir is a save dir
	QFileInfo normalizedPath(mCurrentDir.absoluteFilePath(path)); // absoluteDir?
	QString file = normalizedPath.filePath();
	QFile in(file);
	if (!in.open(QIODevice::ReadOnly | QIODevice::Text)) {
		emit error(tr("Trying to read from file %1 failed").arg(file));
		return {};
	}

	QStringList result;

	while (!in.atEnd()) {
		const auto line = in.readLine();
		result << QString::fromUtf8(line);
	}

	return result;
}

utils::AbstractTimer *TrikBrick::timer(int milliseconds)
{
	utils::AbstractTimer *result = mTwoDRobotModel->timeline().produceTimer();
	mTimers.append(result);
	result->setRepeatable(true); // seems to be the case
	result->start(milliseconds);
	return result;
}

