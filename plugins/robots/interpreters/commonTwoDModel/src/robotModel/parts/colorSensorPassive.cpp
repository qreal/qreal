#include "commonTwoDModel/robotModel/parts/colorSensorPassive.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

ColorSensorPassive::ColorSensorPassive(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, engine::TwoDModelEngineInterface &engine)
	: interpreterBase::robotModel::robotParts::ColorSensorPassive(info, port)
	, mEngine(engine)
{
}

void ColorSensorPassive::read()
{
	emit newData(mEngine.readColorSensor(deviceInfo(), port()));
}
