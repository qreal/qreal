#include "commonTwoDModel/robotModel/parts/marker.h"

using namespace twoDModel::robotModel::parts;

Marker::Marker(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, engine::TwoDModelEngineInterface &engine)
	: Device(info, port)
	, mEngine(engine)
{
}

#include <QDebug>
#include <QColor>
void Marker::prepare(QColor const &color)
{
	qDebug() << "prepared" << color;
}

/// Lifts the marker of the 2D model robot up.
/// The robot stops drawing its trace on the floor after that.
void Marker::remove()
{
	qDebug() << "lifted";
}
