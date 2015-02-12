#include "commonTwoDModel/robotModel/parts/colorSensorGreen.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

ColorSensorGreen::ColorSensorGreen(const interpreterBase::robotModel::DeviceInfo &info
		, const interpreterBase::robotModel::PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: interpreterBase::robotModel::robotParts::ColorSensorGreen(info, port)
	, mEngine(engine)
{
}

void ColorSensorGreen::read()
{
	emit newData(mEngine.readColorSensor(port()));
}
