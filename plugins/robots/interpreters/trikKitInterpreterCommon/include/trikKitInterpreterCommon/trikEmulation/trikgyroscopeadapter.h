#pragma once

#include <trikControl/vectorSensorInterface.h>
#include <kitBase/robotModel/robotParts/gyroscopeSensor.h> // replace with forward declaration

class TrikGyroscopeAdapter : public trikControl::VectorSensorInterface {

Q_OBJECT

public:
	TrikGyroscopeAdapter(kitBase::robotModel::robotParts::GyroscopeSensor *g);

	virtual Status status() const override { return Status::ready; }

public slots:
	virtual QVector<int> read() const override;

private:
	kitBase::robotModel::robotParts::GyroscopeSensor *mGyroscope;

};
