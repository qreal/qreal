#pragma once

#include <trikControl/motorInterface.h>

#include <kitBase/robotModel/robotParts/motor.h>

namespace trik {

class TrikMotorEmu : public trikControl::MotorInterface {
	Q_OBJECT

	// DeviceInterface interface
public:
	TrikMotorEmu(kitBase::robotModel::robotParts::Motor * motor);

	Status status() const override {return Status::ready;}

	// MotorInterface interface
public:
	int minControl() const override {return -100;}
	int maxControl() const override {return 100;}

public slots:
	void setPower(int power, bool constrain) override;
	int power() const override;
	void powerOff() override;

private:
	kitBase::robotModel::robotParts::Motor * mMotor;

};

}
