#include "twoDModel/robotModel/parts/colorSensorPassive.h"

#include "twoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace kitBase::robotModel;

ColorSensorPassive::ColorSensorPassive(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: kitBase::robotModel::robotParts::ColorSensorPassive(info, port)
	, mEngine(engine)
{
}

void ColorSensorPassive::read()
{
	emit newData(mEngine.readColorSensor(port()));
}
