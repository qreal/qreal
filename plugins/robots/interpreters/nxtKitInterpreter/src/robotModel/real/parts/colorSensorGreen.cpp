#include "colorSensorGreen.h"

using namespace nxtKitInterpreter::robotModel::real::parts;

ColorSensorGreen::ColorSensorGreen(const interpreterBase::robotModel::DeviceInfo &info
		, const interpreterBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: interpreterBase::robotModel::robotParts::ColorSensorGreen(info, port)
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
