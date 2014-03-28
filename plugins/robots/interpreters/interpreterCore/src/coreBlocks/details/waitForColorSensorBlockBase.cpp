#include "waitForColorSensorBlockBase.h"

using namespace interpreterCore::coreBlocks::details;

WaitForColorSensorBlockBase::WaitForColorSensorBlockBase(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: WaitForSensorBlock(robotModel)
{
}

WaitForColorSensorBlockBase::~WaitForColorSensorBlockBase()
{
}

//qReal::interpreters::robots::details::robotParts::Sensor *WaitForColorSensorBlockBase::sensor() const
//{
//	return mRobotModel->colorSensor(mPort);
//}

QString WaitForColorSensorBlockBase::name() const
{
	return tr("Color sensor");
}
