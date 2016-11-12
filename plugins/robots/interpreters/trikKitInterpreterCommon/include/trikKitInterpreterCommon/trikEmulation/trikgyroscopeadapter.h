#pragma once

#include <trikControl/vectorSensorInterface.h>

#include <kitBase/robotModel/portInfo.h> // forward?

namespace twoDModel {
namespace engine {
class TwoDModelEngineInterface;
}
}

class TrikGyroscopeAdapter : public trikControl::VectorSensorInterface {

Q_OBJECT

public:
	TrikGyroscopeAdapter(const kitBase::robotModel::PortInfo &port, twoDModel::engine::TwoDModelEngineInterface *engine);

	virtual Status status() const override { return Status::ready; }

public slots:
	virtual QVector<int> read() const override;

private:
	kitBase::robotModel::PortInfo mPort;
	twoDModel::engine::TwoDModelEngineInterface *mEngine;
};
