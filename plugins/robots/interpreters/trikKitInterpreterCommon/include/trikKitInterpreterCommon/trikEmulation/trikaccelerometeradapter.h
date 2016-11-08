#pragma once

#include <trikControl/vectorSensorInterface.h>
#include <kitBase/robotModel/robotParts/accelerometerSensor.h> // replace with forward declaration

class TrikAccelerometerAdapter : public trikControl::VectorSensorInterface {

Q_OBJECT

public:
	TrikAccelerometerAdapter(kitBase::robotModel::robotParts::AccelerometerSensor *acc);

	virtual Status status() const override { return Status::ready; }

public slots:
	virtual QVector<int> read() const override;

private:
	kitBase::robotModel::robotParts::AccelerometerSensor *mAccelerometer;

};
