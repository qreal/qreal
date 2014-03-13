#include "colorSensorFull.h"

using namespace nxtKitInterpreter::robotModel::real::parts;

QString ColorSensorFull::friendlyName()
{
	return tr("Color sensor (full colors)");
}

ColorSensorFull::ColorSensorFull(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: ColorSensor(info, port, robotCommunicator, enums::lowLevelSensorType::COLORFULL)
{
}
