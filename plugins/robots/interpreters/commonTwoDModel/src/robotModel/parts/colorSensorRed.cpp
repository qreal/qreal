#include "commonTwoDModel/robotModel/parts/colorSensorRed.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

ColorSensorRed::ColorSensorRed(const interpreterBase::robotModel::DeviceInfo &info
		, const interpreterBase::robotModel::PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: interpreterBase::robotModel::robotParts::ColorSensorRed(info, port)
	, mEngine(engine)
{
}

void ColorSensorRed::read()
{
	emit newData(mEngine.readColorSensor(port()));
}
