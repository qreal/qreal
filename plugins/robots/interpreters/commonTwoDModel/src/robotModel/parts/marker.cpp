#include "commonTwoDModel/robotModel/parts/marker.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;

Marker::Marker(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, engine::TwoDModelEngineInterface &engine)
	: Device(info, port)
	, mEngine(engine)
{
}

void Marker::down(QColor const &color)
{
	mEngine.markerDown(color);
}

void Marker::up()
{
	mEngine.markerUp();
}
