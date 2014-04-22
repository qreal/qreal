#include "colorSensorPassive.h"

using namespace nxtKitInterpreter::robotModel::real::parts;

ColorSensorPassive::ColorSensorPassive(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: interpreterBase::robotModel::robotParts::ColorSensorPassive(info, port)
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
