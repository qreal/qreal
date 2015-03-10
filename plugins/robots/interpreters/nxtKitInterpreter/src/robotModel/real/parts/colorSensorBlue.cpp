#include "colorSensorBlue.h"

using namespace nxt::robotModel::real::parts;

ColorSensorBlue::ColorSensorBlue(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::ColorSensorBlue(info, port)
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
