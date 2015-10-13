/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "trikKitInterpreterCommon/robotModel/twoD/parts/twoDLineSensor.h"

#include <QtGui/QImage>

using namespace trik::robotModel::twoD::parts;
using namespace kitBase::robotModel;

// The color of the pixel
const int tolerance = 10;

LineSensor::LineSensor(const DeviceInfo &info, const PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikLineSensor(info, port)
	, mEngine(engine)
	, mLineColor(qRgb(0, 0, 0))  // default line color is black
{
}

void LineSensor::init()
{
}

void LineSensor::detectLine()
{
	const QImage image = mEngine.areaUnderSensor(port(), 0.2);

	int size = 0;
	int red = 0;
	int green = 0;
	int blue = 0;
	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			const QRgb pixelColor = image.pixel(x, y);
			if (qAlpha(pixelColor) > 0) {
				++size;
				red += qRed(pixelColor);
				green += qGreen(pixelColor);
				blue += qBlue(pixelColor);
			}
		}
	}

	mLineColor = qRgb(red / size, green / size, blue / size);
}

void LineSensor::read()
{
	const QImage image = mEngine.areaUnderSensor(port(), 2.0);

	const int height = image.height();
	const int width = image.width();

	int blacks = 0;
	int crossBlacks = 0;
	int usefulRows = 0;
	int horizontalLineWidth = image.height() * 0.2;
	qreal xCoordinates = 0;
	for (int i = 0; i < height; ++i) {
		int blacksInRow = 0;
		qreal xSum = 0;

		for (int j = 0; j < width; ++j) {
			if (closeEnough(image.pixel(j, i))) {
				++blacksInRow;
				xSum += j - width / 2.0;
			}
		}

		xCoordinates += (blacksInRow ? xSum * 100 / (width / 2.0) / blacksInRow : 0);
		blacks += blacksInRow;
		usefulRows += blacksInRow ? 1 : 0;
		if (((height - horizontalLineWidth) / 2 < i) && (i < (height + horizontalLineWidth) / 2)) {
			crossBlacks += blacksInRow;
		}
	}

	const int x = qRound(xCoordinates / usefulRows);
	const int lineWidth = blacks / height;
	const int cross = qRound(crossBlacks * 100.0 / (height * horizontalLineWidth));
	emit newData({ x, lineWidth, cross });
}

bool LineSensor::closeEnough(QRgb color) const
{
	return qAlpha(color) > 0 && qMax(qAbs(qRed(color) - qRed(mLineColor))
		, qMax(qAbs(qGreen(color) - qGreen(mLineColor))
		, qAbs(qBlue(color) - qBlue(mLineColor)))) < tolerance;
}
