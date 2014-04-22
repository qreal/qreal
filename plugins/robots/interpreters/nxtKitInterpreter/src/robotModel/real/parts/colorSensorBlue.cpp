#include "colorSensorBlue.h"

using namespace nxtKitInterpreter::robotModel::real::parts;

ColorSensorBlue::ColorSensorBlue(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: interpreterBase::robotModel::robotParts::ColorSensorBlue(info, port)
	, mImpl(port, robotCommunicator, enums::lowLevelSensorType::COLORBLUE)
{
	connect(&mImpl, &ColorSensorImpl::newData, this, &ColorSensorBlue::newData);
	connect(&mImpl, &ColorSensorImpl::configurationCompleted, this, &ColorSensorBlue::configurationCompleted);
	connect(&mImpl, &ColorSensorImpl::failure, this, &ColorSensorBlue::failure);
}

void ColorSensorBlue::read()
{
	mImpl.read();
}

void ColorSensorBlue::doConfiguration()
{
	mImpl.doConfiguration();
}
