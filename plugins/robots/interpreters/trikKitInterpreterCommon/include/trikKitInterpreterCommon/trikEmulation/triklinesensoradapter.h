#pragma once

#include <trikControl/lineSensorInterface.h>
#include <trikKit/robotModel/parts/trikLineSensor.h> // replace with forward declaration

class TrikLineSensorAdapter : public trikControl::LineSensorInterface {

Q_OBJECT

public:
	TrikLineSensorAdapter(trik::robotModel::parts::TrikLineSensor *sensor);
	virtual Status status() const override;

public slots:
	virtual void init(bool showOnDisplay) override;
	virtual void detect() override;
	virtual QVector<int> read() override;
	virtual void stop() override;
	virtual QVector<int> getDetectParameters() const override;

private:

	trik::robotModel::parts::TrikLineSensor *mLineSensor;
};
