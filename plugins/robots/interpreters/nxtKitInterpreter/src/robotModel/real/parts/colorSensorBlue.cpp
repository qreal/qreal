#include "colorSensorBlue.h"

using namespace nxtKitInterpreter::robotModel::real::parts;

QString ColorSensorBlue::friendlyName()
{
	return tr("Color sensor (blue)");
}

ColorSensorBlue::ColorSensorBlue(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: ColorSensor(info, port, robotCommunicator, enums::lowLevelSensorType::COLORBLUE)
{
}
