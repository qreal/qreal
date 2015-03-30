#include "twoDLineSensor.h"

#include <QtGui/QImage>

using namespace trik::robotModel::twoD::parts;
using namespace kitBase::robotModel;



LineSensor::LineSensor(const DeviceInfo &info, const PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikLineSensor(info, port)
	, mEngine(engine)
	, mLineColor(Qt::black)
{
}

void LineSensor::init()
{
}

void LineSensor::detectLine()
{
	QImage image = mEngine.areaUnderSensor(port(), 0.2);

	int size = image.width() * image.height();
	int red = 0;
	int green = 0;
	int blue = 0;
	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			QRgb pixelColor(image.pixel(x, y));
			red += qRed(pixelColor);
			green += qGreen(pixelColor);
			blue += qBlue(pixelColor);
		}
	}

	mLineColor.setRed(red / size);
	mLineColor.setGreen(green / size);
	mLineColor.setBlue(blue / size);
}

void LineSensor::read()
{
	QImage image = mEngine.areaUnderSensor(port(), 2.0);

	int height = image.height();
	int width = image.width();

	int blacks = 0;
	int crossBlacks = 0;
	int horizontalLineWidth = image.height() * 0.2;
	qreal xCoordinates = 0;
	for (int i = 0; i < height; ++i) {
		int blacksInRow = 0;
		qreal xSum = 0;

		for (int j = 0; j < width; ++j) {
			if (closeEnough(image.pixel(j, i))) {
				++blacksInRow;
				xSum += (j + 1) * 100.0 / (width / 2.0) - 100;
			}
		}

		xCoordinates += (blacksInRow ? xSum / blacksInRow : 0);
		blacks += blacksInRow;
		if (((height - horizontalLineWidth) / 2 < i) && (i < (height + horizontalLineWidth) / 2)) {
			crossBlacks += blacksInRow;
		}
	}

	const int x = qRound(xCoordinates / height);
	const int lineWidth = blacks / height;
	const int cross = qRound(crossBlacks * 100.0 / (height * horizontalLineWidth));
	emit newData({ x, lineWidth, cross });
}

bool LineSensor::closeEnough(const QColor &color) const
{
	return qMax(abs(color.red() - mLineColor.red()), qMax(abs(color.green() - mLineColor.green())
		, abs(color.blue() - mLineColor.blue()))) < 10;
	return color.red() < 10 && color.green() < 10 && color.blue() < 10;
}
