#pragma once

#include <trikControl/encoderInterface.h>

#include <kitBase/robotModel/robotParts/encoderSensor.h>

namespace trik {

class TrikEncoderAdapter : public trikControl::EncoderInterface
{
	Q_OBJECT
	// DeviceInterface interface
public:
	TrikEncoderAdapter(kitBase::robotModel::robotParts::EncoderSensor *encoder);

	virtual Status status() const override {return Status::ready;}
	// EncoderInterface interface
public slots:
	virtual int read() override;
	virtual int readRawData() override { return read(); }
	virtual void reset() override;

private:
	kitBase::robotModel::robotParts::EncoderSensor *mEncoder;
};

}
