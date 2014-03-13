#include "colorSensorPassive.h"

using namespace nxtKitInterpreter::robotModel::real::parts;

QString ColorSensorPassive::friendlyName()
{
	return tr("Color sensor (passive)");
}

ColorSensorPassive::ColorSensorPassive(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: ColorSensor(info, port, robotCommunicator, enums::lowLevelSensorType::COLORNONE)
{
}
