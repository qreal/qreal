#include "colorSensorFull.h"

using namespace nxtKitInterpreter::robotModel::real::parts;

ColorSensorFull::ColorSensorFull(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: ColorSensor(info, port, robotCommunicator, enums::lowLevelSensorType::COLORFULL)
{
}
