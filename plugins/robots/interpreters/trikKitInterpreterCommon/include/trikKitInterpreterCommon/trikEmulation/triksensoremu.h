#pragma once

#include <trikControl/sensorInterface.h>

#include <kitBase/robotModel/robotParts/scalarSensor.h>

namespace trik {

class TrikSensorEmu : public trikControl::SensorInterface
{
	Q_OBJECT

	// DeviceInterface interface
public:
	TrikSensorEmu(kitBase::robotModel::robotParts::ScalarSensor * sensor);

	Status status() const override {return Status::ready;}

	// SensorInterface interface
public:
	int minValue() const override {return 0;}
	int maxValue() const override {return 100;}

public slots:
	int read() override;
	int readRawData() override {return read();}

private:
	kitBase::robotModel::robotParts::ScalarSensor *mSensor;

};

}
