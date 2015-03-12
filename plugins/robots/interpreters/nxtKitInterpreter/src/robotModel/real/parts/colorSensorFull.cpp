#include "colorSensorFull.h"

using namespace nxt::robotModel::real::parts;

ColorSensorFull::ColorSensorFull(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::ColorSensorFull(info, port)
	, mImpl(port, robotCommunicator, enums::lowLevelSensorType::COLORFULL)
{
	connect(&mImpl, &ColorSensorImpl::newData, this, &ColorSensorFull::newData);
	connect(&mImpl, &ColorSensorImpl::configurationCompleted, this, &ColorSensorFull::configurationCompleted);
	connect(&mImpl, &ColorSensorImpl::failure, this, &ColorSensorFull::failure);
}

void ColorSensorFull::read()
{
	mImpl.read();
}

void ColorSensorFull::doConfiguration()
{
	mImpl.doConfiguration();
}
