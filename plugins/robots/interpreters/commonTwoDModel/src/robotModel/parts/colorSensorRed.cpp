#include "commonTwoDModel/robotModel/parts/colorSensorRed.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

ColorSensorRed::ColorSensorRed(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, engine::TwoDModelEngineInterface &engine)
	: interpreterBase::robotModel::robotParts::ColorSensorRed(info, port)
	, mEngine(engine)
{
}

void ColorSensorRed::read()
{
	emit newData(mEngine.readColorSensor(deviceInfo(), port()));
}
