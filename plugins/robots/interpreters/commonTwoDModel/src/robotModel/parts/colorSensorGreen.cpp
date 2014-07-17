#include "commonTwoDModel/robotModel/parts/colorSensorGreen.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

ColorSensorGreen::ColorSensorGreen(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, engine::TwoDModelEngineInterface &engine)
	: interpreterBase::robotModel::robotParts::ColorSensorGreen(info, port)
	, mEngine(engine)
{
}

void ColorSensorGreen::read()
{
	emit newData(mEngine.readColorSensor(deviceInfo(), port()));
}
