#pragma once

#include <trikKit/robotModel/parts/trikLed.h>
#include <twoDModel/engine/twoDModelEngineInterface.h>

namespace trik {
namespace robotModel {
namespace twoD {
namespace parts {

class TwoDLed : public robotModel::parts::TrikLed
{
	Q_OBJECT

public:
	TwoDLed(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	/// sets LED widget's color in 2D model.
	void setColor(const QString &color) override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
};

}
}
}
}
