#pragma once

#include <trikControl/brickInterface.h>
/// TODO: factor everything out to trikit common, leave only textinterp.
#include <kitBase/robotModel/robotModelInterface.h>

class TrikBrick : public trikControl::BrickInterface {


	// BrickInterface interface
public:
	TrikBrick(kitBase::robotModel::RobotModelInterface *model);
	void reset() override {}
	trikControl::DisplayWidgetInterface &graphicsWidget() override {
		trikControl::DisplayWidgetInterface temp;
		return temp;
	}

public slots:
	void configure(const QString &portName, const QString &deviceName) override {}
	void playSound(const QString &soundFileName) override {}
	void say(const QString &text) override {}
	void stop() override {}
	trikControl::MotorInterface *motor(const QString &port) override {return nullptr;}
	trikControl::PwmCaptureInterface *pwmCapture(const QString &port) override {return nullptr;}
	trikControl::SensorInterface *sensor(const QString &port) override {return nullptr;}
	QStringList motorPorts(trikControl::MotorInterface::Type type) const override {return {};}
	QStringList pwmCapturePorts() const override {return {};}
	QStringList sensorPorts(trikControl::SensorInterface::Type type) const override {return {};}
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

private:
	kitBase::robotModel::RobotModelInterface *mTwoDModel;

};
