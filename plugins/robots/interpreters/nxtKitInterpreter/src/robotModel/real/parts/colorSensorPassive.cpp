#include "colorSensorPassive.h"

using namespace nxt::robotModel::real::parts;

ColorSensorPassive::ColorSensorPassive(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::ColorSensorPassive(info, port)
	, mImpl(port, robotCommunicator, enums::lowLevelSensorType::COLORNONE)
{
	connect(&mImpl, &ColorSensorImpl::newData, this, &ColorSensorPassive::newData);
	connect(&mImpl, &ColorSensorImpl::configurationCompleted, this, &ColorSensorPassive::configurationCompleted);
	connect(&mImpl, &ColorSensorImpl::failure, this, &ColorSensorPassive::failure);
}

void ColorSensorPassive::read()
{
	mImpl.read();
}

void ColorSensorPassive::doConfiguration()
{
	mImpl.doConfiguration();
}
