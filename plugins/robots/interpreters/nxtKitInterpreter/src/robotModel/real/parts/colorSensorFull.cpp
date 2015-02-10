#include "colorSensorFull.h"

using namespace nxtKitInterpreter::robotModel::real::parts;

ColorSensorFull::ColorSensorFull(const interpreterBase::robotModel::DeviceInfo &info
		, const interpreterBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: interpreterBase::robotModel::robotParts::ColorSensorFull(info, port)
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
