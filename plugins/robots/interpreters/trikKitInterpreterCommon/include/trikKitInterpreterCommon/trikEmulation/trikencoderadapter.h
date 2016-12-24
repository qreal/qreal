#pragma once

#include <trikControl/encoderInterface.h>

#include <kitBase/robotModel/portInfo.h> // forward?

namespace twoDModel {
namespace engine {
class TwoDModelEngineInterface;
}
}

namespace trik {

class TrikEncoderAdapter : public trikControl::EncoderInterface
{
	Q_OBJECT
	// DeviceInterface interface
public:
	TrikEncoderAdapter(const kitBase::robotModel::PortInfo &port, twoDModel::engine::TwoDModelEngineInterface *engine);

	virtual Status status() const override {return Status::ready;}
	// EncoderInterface interface
public slots:
	virtual int read() override;
	virtual int readRawData() override { return read(); }
	virtual void reset() override;

private:
	kitBase::robotModel::PortInfo mPort;
	twoDModel::engine::TwoDModelEngineInterface *mEngine;
};

}
