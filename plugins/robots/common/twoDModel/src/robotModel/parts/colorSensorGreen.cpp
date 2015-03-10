#include "twoDModel/robotModel/parts/colorSensorGreen.h"

#include "twoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace kitBase::robotModel;

ColorSensorGreen::ColorSensorGreen(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: kitBase::robotModel::robotParts::ColorSensorGreen(info, port)
	, mEngine(engine)
{
}

void ColorSensorGreen::read()
{
	emit newData(mEngine.readColorSensor(port()));
}
