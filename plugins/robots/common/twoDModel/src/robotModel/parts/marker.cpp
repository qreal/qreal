#include "twoDModel/robotModel/parts/marker.h"

#include "twoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;

Marker::Marker(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: Device(info, port)
	, mEngine(engine)
{
}

void Marker::down(const QColor &color)
{
	mEngine.markerDown(color);
}

void Marker::up()
{
	mEngine.markerUp();
}
