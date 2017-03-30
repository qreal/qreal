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

#pragma once

#include <trikControl/motorInterface.h>

#include <kitBase/robotModel/robotParts/motor.h>

namespace trik {

class TrikMotorEmu : public trikControl::MotorInterface
{
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
	void forceBreak(int) override;

private:
	kitBase::robotModel::robotParts::Motor * mMotor;

};

}
