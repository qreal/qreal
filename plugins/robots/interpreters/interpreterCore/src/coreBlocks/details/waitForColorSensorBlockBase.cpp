#include "waitForColorSensorBlockBase.h"

using namespace qReal::interpreters::robots::details::blocks;

WaitForColorSensorBlockBase::WaitForColorSensorBlockBase(details::RobotModel * const robotModel
		, robots::enums::sensorType::SensorTypeEnum type)
	: WaitForSensorBlock(robotModel, type)
{
}

qReal::interpreters::robots::details::robotParts::Sensor *WaitForColorSensorBlockBase::sensor() const
{
	return mRobotModel->colorSensor(mPort);
}

QString WaitForColorSensorBlockBase::name() const
{
	return tr("Color sensor");
}
