#include "twoDLineSensor.h"

#include <QtGui/QImage>

using namespace trik::robotModel::twoD::parts;
using namespace kitBase::robotModel;

LineSensor::LineSensor(const DeviceInfo &info, const PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikLineSensor(info, port)
	, mEngine(engine)
{
}

void LineSensor::init()
{
}

void LineSensor::detectLine()
{
	QImage image = mEngine.areaUnderSensor(port(), 0.2);
	int size = image.width() * image.height();
	qreal red = 0;
	qreal green = 0;
	qreal blue = 0;
	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			QColor pixelColor(image.pixel(x, y));
			red += pixelColor.redF();
			green += pixelColor.greenF();
			blue += pixelColor.blueF();
		}
	}

	mLineColor.setRedF(red / size);
	mLineColor.setGreenF(green / size);
	mLineColor.setBlueF(blue / size);
}

void LineSensor::read()
{
	QImage image = mEngine.areaUnderSensor(port(), 2.0);
	emit newData({0, 0, 0});
}
