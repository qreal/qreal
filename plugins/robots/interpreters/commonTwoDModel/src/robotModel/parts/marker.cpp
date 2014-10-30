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

void Marker::prepare(QColor const &color)
{
	mEngine.prepareMarker(color);
}

void Marker::remove()
{
	mEngine.removeMarker();
}
