#pragma once

#include <interpreterBase/robotModel/portInfo.h>
#include <interpreterBase/robotModel/robotParts/motor.h>

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

class Motor : public interpreterBase::robotModel::robotParts::Motor
{
	Q_OBJECT

public:
	static QString friendlyName();

	Motor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, engine::TwoDModelEngineInterface &engine
			);

	void on(int speed, bool breakMode = true) override;
	void on(int speed, long unsigned int degrees, bool breakMode = true) override;
	void stop(bool breakMode = true) override;
	void off() override;
//	void resetMotorPosition(bool relative);

protected:
	void doConfiguration() override;
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}
