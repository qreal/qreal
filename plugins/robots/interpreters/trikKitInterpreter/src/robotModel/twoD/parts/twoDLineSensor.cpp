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

	int height = image.height();
	int width = image.width();

	int blacks = 0;
	int horizontalBlacks = 0;
	int horizontalLineWidth = image.height() * 0.2;
	int xCoordinates = 0;
	QVector<QVector<bool> > bitMap(height, QVector<bool>(width, false));
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			bitMap[i][j] = closeEnough(image.pixel(j, i));
			if (bitMap[i][j]) {
				++blacks;
				xCoordinates += i;
				if (((height - horizontalLineWidth) / 2 < i)
						&& (i < (height + horizontalLineWidth) / 2)) {
					++horizontalBlacks;
				}
			}
		}
	}

	emit newData({
			xCoordinates / blacks
			, blacks / (height * width - blacks)
			, horizontalBlacks / (height * horizontalLineWidth)
	});
}

bool LineSensor::closeEnough(const QColor &color) const
{
	return qMax(abs(color.red() - mLineColor.red()), qMax(abs(color.green() - mLineColor.green())
		, abs(color.blue() - mLineColor.blue()))) < 10;
}
