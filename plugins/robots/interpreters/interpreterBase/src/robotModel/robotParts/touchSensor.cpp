#include "interpreterBase/robotModel/robotParts/touchSensor.h"

#include <QtCore/QDebug>

using namespace interpreterBase::robotModel::robotParts;

void TouchSensor::read()
{
	emit newData(mCount++ < 100 ? 0 : 1);
}

void TouchSensor::doConfiguration()
{
	configurationCompleted(true);
}
