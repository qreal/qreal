#pragma once

#include <QHash>

#include <trikControl/brickInterface.h>

#include "robotModel/twoD/trikTwoDRobotModel.h"

#include "trikEmulation/trikdisplayemu.h"
#include "trikEmulation/triksensoremu.h"
#include "trikEmulation/trikmotoremu.h"

namespace trik {

class TrikBrick : public trikControl::BrickInterface
{
	Q_OBJECT

	// BrickInterface interface
public:
	TrikBrick(const QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> &model);
	void reset() override {}
	QString configVersion() const {return QString();}
	trikControl::DisplayWidgetInterface &graphicsWidget() override {
		trikControl::DisplayWidgetInterface temp;
		return temp;
	}
	void init();

public slots:
	void configure(const QString &portName, const QString &deviceName) override {}
	void playSound(const QString &soundFileName) override {}
	void say(const QString &text) override {}
	void stop() override {}
	trikControl::MotorInterface *motor(const QString &port) override;
	trikControl::PwmCaptureInterface *pwmCapture(const QString &port) override {return nullptr;}
	trikControl::SensorInterface *sensor(const QString &port) override;
	QStringList motorPorts(trikControl::MotorInterface::Type type) const override;
	QStringList pwmCapturePorts() const override {return {};}
	QStringList sensorPorts(trikControl::SensorInterface::Type type) const override;
	QStringList encoderPorts() const override {return {};}
	trikControl::VectorSensorInterface *accelerometer() override {return nullptr;}
	trikControl::VectorSensorInterface *gyroscope() override {return nullptr;}
	trikControl::LineSensorInterface *lineSensor(const QString &port) override {return nullptr;}
	trikControl::ColorSensorInterface *colorSensor(const QString &port) override {return nullptr;}
	trikControl::ObjectSensorInterface *objectSensor(const QString &port) override {return nullptr;}
	trikControl::SoundSensorInterface *soundSensor(const QString &port) override {return nullptr;}
	trikControl::EncoderInterface *encoder(const QString &port) override {return nullptr;}
	trikControl::BatteryInterface *battery() override {return nullptr;}
	trikControl::KeysInterface *keys() override {return nullptr;}
	trikControl::DisplayInterface *display() override;
	trikControl::LedInterface *led() override {return nullptr;}
	trikControl::FifoInterface *fifo(const QString &port) override {return nullptr;}

signals:
	void error(const QString &msg);

private:

	QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> mTwoDRobotModel;

	TrikDisplayEmu mDisplay;

	QHash<QString, TrikMotorEmu *> mMotors;
	QHash<QString, TrikSensorEmu *> mSensors;

};

}
