#include "colorSensorBlue.h"

using namespace nxtKitInterpreter::robotModel::real::parts;

ColorSensorBlue::ColorSensorBlue(const interpreterBase::robotModel::DeviceInfo &info
		, const interpreterBase::robotModel::PortInfo &port
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
