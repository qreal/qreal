#include "colorSensorRed.h"

using namespace nxtKitInterpreter::robotModel::real::parts;

QString ColorSensorRed::friendlyName()
{
	return tr("Color sensor (red)");
}

ColorSensorRed::ColorSensorRed(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: ColorSensor(info, port, robotCommunicator, enums::lowLevelSensorType::COLORRED)
{
}
