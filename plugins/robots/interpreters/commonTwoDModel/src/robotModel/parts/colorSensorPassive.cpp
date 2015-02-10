#include "commonTwoDModel/robotModel/parts/colorSensorPassive.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

ColorSensorPassive::ColorSensorPassive(const interpreterBase::robotModel::DeviceInfo &info
		, const interpreterBase::robotModel::PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: interpreterBase::robotModel::robotParts::ColorSensorPassive(info, port)
	, mEngine(engine)
{
}

void ColorSensorPassive::read()
{
	emit newData(mEngine.readColorSensor(port()));
}
