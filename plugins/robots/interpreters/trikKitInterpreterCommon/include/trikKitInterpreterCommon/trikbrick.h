#pragma once

#include <QtCore/QHash>
#include <QtCore/QScopedPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QDir>

#include <trikControl/brickInterface.h>

#include <trikKitInterpreterCommon/robotModel/twoD/trikTwoDRobotModel.h>

#include "trikEmulation/trikdisplayemu.h"
#include "trikEmulation/triksensoremu.h"
#include "trikEmulation/trikencoderadapter.h"
#include "trikEmulation/trikmotoremu.h"
#include "trikEmulation/trikkeysinterfacestub.h"
#include "trikEmulation/triklinesensoradapter.h"
#include "trikEmulation/trikledadapter.h"
#include "trikEmulation/trikaccelerometeradapter.h"
#include "trikEmulation/trikgyroscopeadapter.h" /// @todo: replace with forward refs

namespace trik {

class TrikBrick final : public trikControl::BrickInterface
{
	Q_OBJECT

	// BrickInterface interface
public:
	TrikBrick(const QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> &model);
	~TrikBrick();
	void reset() override;
	QString configVersion() const {return QString();}
	trikControl::DisplayWidgetInterface *graphicsWidget() override {
		return nullptr;
	}
	void init();

	void setCurrentDir(const QString &dir);
	void setCurrentInputs(const QString &f);

public slots:
	void configure(const QString &portName, const QString &deviceName) override {}
	void playSound(const QString &soundFileName) override {}
	void say(const QString &text) override {}
	void stop() override;
	trikControl::MotorInterface *motor(const QString &port) override;
	trikControl::PwmCaptureInterface *pwmCapture(const QString &port) override {return nullptr;}
	trikControl::SensorInterface *sensor(const QString &port) override;
	QStringList motorPorts(trikControl::MotorInterface::Type type) const override;
	QStringList pwmCapturePorts() const override {return {};}
	QStringList sensorPorts(trikControl::SensorInterface::Type type) const override;
	QStringList encoderPorts() const override;
	trikControl::VectorSensorInterface *accelerometer() override;
	trikControl::GyroSensorInterface *gyroscope() override;
	trikControl::LineSensorInterface *lineSensor(const QString &port) override;
	trikControl::ColorSensorInterface *colorSensor(const QString &port) override {return nullptr;}
	trikControl::ObjectSensorInterface *objectSensor(const QString &port) override {return nullptr;}
	trikControl::SoundSensorInterface *soundSensor(const QString &port) override {return nullptr;}
	trikControl::EncoderInterface *encoder(const QString &port) override;
	trikControl::BatteryInterface *battery() override {return nullptr;}
	trikControl::KeysInterface *keys() override {return &mKeys;}
	trikControl::DisplayInterface *display() override;
	trikControl::LedInterface *led() override;
	trikControl::GamepadInterface *gamepad() override {return nullptr;}
	trikControl::FifoInterface *fifo(const QString &port) override {return nullptr;}

	void playTone(int hzFreq, int msDuration) override {}
	trikControl::EventDeviceInterface *eventDevice(const QString &deviceFile) override {return nullptr;}
	void stopEventDevice(const QString &deviceFile) override {}

	/// some ScriptExecution control replacements. @todo: factor out in the separate class
	int random(int from, int to);
	void wait(int milliseconds);
	qint64 time() const;
	QStringList readAll(const QString &path);
signals:
	void error(const QString &msg);
	void log(const QString &msg);
	void stopWaiting();

private:
	void printToShell(const QString &msg);

	QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> mTwoDRobotModel;

	TrikDisplayEmu mDisplay;
	TrikKeysInterfaceStub mKeys;

	bool mIsWaitingEnabled;

	QTimer mSensorUpdater;

	QHash<QString, TrikMotorEmu *> mMotors;
	QHash<QString, TrikSensorEmu *> mSensors;
	QHash<QString, TrikEncoderAdapter *> mEncoders;
	QHash<QString, TrikLineSensorAdapter *> mLineSensors;
	QScopedPointer<TrikLedAdapter> mLed;
	QScopedPointer<TrikAccelerometerAdapter> mAccelerometer;
	QScopedPointer<TrikGyroscopeAdapter> mGyroscope;

	QDir mCurrentDir;
	bool mIsExcerciseMode = false;
	QStringList mInputs;

};

}
