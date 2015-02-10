#include "colorSensorRed.h"

using namespace nxtKitInterpreter::robotModel::real::parts;

ColorSensorRed::ColorSensorRed(const interpreterBase::robotModel::DeviceInfo &info
		, const interpreterBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: interpreterBase::robotModel::robotParts::ColorSensorRed(info, port)
	, mImpl(port, robotCommunicator, enums::lowLevelSensorType::COLORRED)
{
	connect(&mImpl, &ColorSensorImpl::newData, this, &ColorSensorRed::newData);
	connect(&mImpl, &ColorSensorImpl::configurationCompleted, this, &ColorSensorRed::configurationCompleted);
	connect(&mImpl, &ColorSensorImpl::failure, this, &ColorSensorRed::failure);
}

void ColorSensorRed::read()
{
	mImpl.read();
}

void ColorSensorRed::doConfiguration()
{
	mImpl.doConfiguration();
}
