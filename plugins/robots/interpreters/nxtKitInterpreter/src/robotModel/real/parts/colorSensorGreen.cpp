#include "colorSensorGreen.h"

using namespace nxt::robotModel::real::parts;

ColorSensorGreen::ColorSensorGreen(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::ColorSensorGreen(info, port)
	, mImpl(port, robotCommunicator, enums::lowLevelSensorType::COLORGREEN)
{
	connect(&mImpl, &ColorSensorImpl::newData, this, &ColorSensorGreen::newData);
	connect(&mImpl, &ColorSensorImpl::configurationCompleted, this, &ColorSensorGreen::configurationCompleted);
	connect(&mImpl, &ColorSensorImpl::failure, this, &ColorSensorGreen::failure);
}

void ColorSensorGreen::read()
{
	mImpl.read();
}

void ColorSensorGreen::doConfiguration()
{
	mImpl.doConfiguration();
}
