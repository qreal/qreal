#include "twoDModelEngineApi.h"

#include <QtCore/QDebug>
#include <QtCore/qmath.h>

#include <qrkernel/settingsManager.h>
#include <qrutils/mathUtils/math.h>
/// @todo: Get rid of it!
#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <interpreterBase/robotModel/robotParts/colorSensorFull.h>
#include <interpreterBase/robotModel/robotParts/colorSensorPassive.h>
#include <interpreterBase/robotModel/robotParts/colorSensorRed.h>
#include <interpreterBase/robotModel/robotParts/colorSensorGreen.h>
#include <interpreterBase/robotModel/robotParts/colorSensorBlue.h>

#include "model/model.h"
#include "model/constants.h"

#include "view/d2ModelWidget.h"

using namespace twoDModel;
using namespace interpreterBase::robotModel;
using namespace twoDModel::model;

TwoDModelEngineApi::TwoDModelEngineApi(model::Model &model, view::D2ModelWidget &view, Configurer const * const configurer)
	: mModel(model)
	, mView(view)
	, mConfigurer(configurer)
{
}

#include <QtWidgets/QGraphicsPathItem>

QGraphicsPathItem *mSensorPath = nullptr;

void TwoDModelEngineApi::setNewMotor(int speed, uint degrees, PortInfo const &port, bool breakMode)
{
	mModel.robotModel().setNewMotor(speed, degrees, port, breakMode);
}

int TwoDModelEngineApi::readEncoder(PortInfo const &port) const
{
	return mModel.robotModel().readEncoder(port);
}

void TwoDModelEngineApi::resetEncoder(PortInfo const &port)
{
	mModel.robotModel().resetEncoder(port);
}

int TwoDModelEngineApi::readTouchSensor(PortInfo const &port) const
{
	if (!mModel.robotModel().configuration().type(port).isA<robotParts::TouchSensor>()) {
		return touchSensorNotPressedSignal;
	}

	QPair<QPointF, qreal> const neededPosDir = countPositionAndDirection(port);
	QPointF sensorPos(neededPosDir.first);
	qreal sensorRotated(neededPosDir.second);

	QPainterPath sensorPath;

	QSizeF const size =  mModel.robotModel().sensorPath(port, sensorPos).size();

	QPointF ellipse = QPointF(size.width() / 2 * cos(sensorRotated / 180 * 3.14), size.width() / 2 * sin(sensorRotated / 180 * 3.14));
	sensorPath.addEllipse(sensorPos + ellipse, size.height() / 2, size.height() / 2);

	bool const res = mModel.worldModel().checkCollision(sensorPath);

	return res ? touchSensorPressedSignal : touchSensorNotPressedSignal;
}

int TwoDModelEngineApi::readSonarSensor(PortInfo const &port) const
{
	QPair<QPointF, qreal> neededPosDir = countPositionAndDirection(port);
	int const res = mModel.worldModel().sonarReading(neededPosDir.first, neededPosDir.second);

	return mModel.settings().realisticSensors() ? spoilSonarReading(res) : res;
}

int TwoDModelEngineApi::spoilSonarReading(int const distance) const
{
	qreal const ran = mathUtils::Math::gaussianNoise(spoilSonarDispersion);
	return mathUtils::Math::truncateToInterval(0, 255, round(distance + ran));
}

int TwoDModelEngineApi::readColorSensor(DeviceInfo const &device, PortInfo const &port) const
{
	QImage const image = printColorSensor(device, port);
	QHash<uint, int> countsColor;

	uint const *data = reinterpret_cast<uint const *>(image.bits());
	int const n = image.byteCount() / 4;
	for (int i = 0; i < n; ++i) {
		uint const color = mModel.settings().realisticSensors() ? spoilColor(data[i]) : data[i];
		++countsColor[color];
	}

	if (mModel.robotModel().configuration().type(port).isA<robotParts::ColorSensorFull>()) {
		return readColorFullSensor(countsColor);
	} else if (mModel.robotModel().configuration().type(port).isA<robotParts::ColorSensorPassive>()) {
		return readColorNoneSensor(countsColor, n);
	} else if (mModel.robotModel().configuration().type(port).isA<robotParts::ColorSensorRed>()) {
		return readSingleColorSensor(red, countsColor, n);
	} else if (mModel.robotModel().configuration().type(port).isA<robotParts::ColorSensorGreen>()) {
		return readSingleColorSensor(green, countsColor, n);
	} else if (mModel.robotModel().configuration().type(port).isA<robotParts::ColorSensorBlue>()) {
		return readSingleColorSensor(blue, countsColor, n);
	}

	qDebug() << "Incorrect 2d model sensor configuration";
	return 0;
}

uint TwoDModelEngineApi::spoilColor(uint const color) const
{
	qreal const noise = mathUtils::Math::gaussianNoise(spoilColorDispersion);

	int r = round(((color >> 16) & 0xFF) + noise);
	int g = round(((color >> 8) & 0xFF) + noise);
	int b = round(((color >> 0) & 0xFF) + noise);
	int const a = (color >> 24) & 0xFF;

	r = mathUtils::Math::truncateToInterval(0, 255, r);
	g = mathUtils::Math::truncateToInterval(0, 255, g);
	b = mathUtils::Math::truncateToInterval(0, 255, b);

	return ((r & 0xFF) << 16) + ((g & 0xFF) << 8) + (b & 0xFF) + ((a & 0xFF) << 24);
}

QImage TwoDModelEngineApi::printColorSensor(DeviceInfo const &device, PortInfo const &port) const
{
	if (mModel.robotModel().configuration().type(port).isNull()) {
		return QImage();
	}

	QPair<QPointF, qreal> const neededPosDir = countPositionAndDirection(port);
	QPointF const position = neededPosDir.first;
	qreal const width =  mConfigurer->sensorImageRect(device).width() / 2.0;
	QRectF const scanningRect = QRectF(position.x() - width, position.y() - width, 2 * width, 2 * width);

	QImage image(scanningRect.size().toSize(), QImage::Format_RGB32);
	QPainter painter(&image);

	QBrush brush(Qt::SolidPattern);
	brush.setColor(Qt::white);
	painter.setBrush(brush);
	painter.setPen(QPen(Qt::white));
	painter.drawRect(scanningRect.translated(-scanningRect.topLeft()));

	bool const wasSelected = mView.sensorItem(port)->isSelected();
	mView.setSensorVisible(port, false);
	mView.scene()->render(&painter, QRectF(), scanningRect);
	mView.setSensorVisible(port, true);
	mView.sensorItem(port)->setSelected(wasSelected);

	return image;
}

int TwoDModelEngineApi::readColorFullSensor(QHash<uint, int> const &countsColor) const
{
	if (countsColor.isEmpty()) {
		return 0;
	}

	QList<int> const values = countsColor.values();
	int maxValue = INT_MIN;
	for (int value : values) {
		if (value > maxValue) {
			maxValue = value;
		}
	}

	uint const maxColor = countsColor.key(maxValue);
	switch (maxColor) {
	case (black):
		return 1;
	case (red):
		return 5;
	case (green):
		return 3;
	case (blue) :
		return 2;
	case (yellow):
		return 4;
	case (white):
		return 6;
	case (cyan):
		return 7;
	case (magenta):
		return 8;
	default:
		return 0;
	}
}

int TwoDModelEngineApi::readSingleColorSensor(uint color, QHash<uint, int> const &countsColor, int n) const
{
	return (static_cast<double>(countsColor[color]) / static_cast<double>(n)) * 100.0;
}

int TwoDModelEngineApi::readColorNoneSensor(QHash<uint, int> const &countsColor, int n) const
{
	double allWhite = static_cast<double>(countsColor[white]);

	QHashIterator<uint, int> i(countsColor);
	while(i.hasNext()) {
		i.next();
		uint const color = i.key();
		if (color != white) {
			int const b = (color >> 0) & 0xFF;
			int const g = (color >> 8) & 0xFF;
			int const r = (color >> 16) & 0xFF;
			qreal const k = qSqrt(static_cast<qreal>(b * b + g * g + r * r)) / 500.0;
			allWhite += static_cast<qreal>(i.value()) * k;
		}
	}

	return (allWhite / static_cast<qreal>(n)) * 100.0;
}

int TwoDModelEngineApi::readLightSensor(DeviceInfo const &device, PortInfo const &port) const
{
	// Must return 1023 on white and 0 on black normalized to percents
	// http://stackoverflow.com/questions/596216/formula-to-determine-brightness-of-rgb-color

	QImage const image = printColorSensor(device, port);
	if (image.isNull()) {
		return 0;
	}

	uint sum = 0;
	uint const *data = reinterpret_cast<uint const *>(image.bits());
	int const n = image.byteCount() / 4;

	for (int i = 0; i < n; ++i) {
		int const color = mModel.settings().realisticSensors() ? spoilLight(data[i]) : data[i];
		int const b = (color >> 0) & 0xFF;
		int const g = (color >> 8) & 0xFF;
		int const r = (color >> 16) & 0xFF;
		// brightness in [0..256]
		int const brightness = 0.2126 * r + 0.7152 * g + 0.0722 * b;

		sum += 4 * brightness; // 4 = max sensor value / max brightness value
	}
	qreal const rawValue = sum / n; // Average by whole region
	return rawValue * 100 / maxLightSensorValur; // Normalizing to percents
}

void TwoDModelEngineApi::playSound(int timeInMs)
{
	mModel.robotModel().playSound(timeInMs);
}

utils::TimelineInterface &TwoDModelEngineApi::modelTimeline()
{
	return mModel.timeline();
}

engine::TwoDModelDisplayInterface *TwoDModelEngineApi::display()
{
	return mView.display();
}

uint TwoDModelEngineApi::spoilLight(uint const color) const
{
	qreal const noise = mathUtils::Math::gaussianNoise(spoilLightDispersion);

	if (noise > (1.0 - percentSaltPepperNoise / 100.0)) {
		return white;
	} else if (noise < (-1.0 + percentSaltPepperNoise / 100.0)) {
		return black;
	}

	return color;
}

QPair<QPointF, qreal> TwoDModelEngineApi::countPositionAndDirection(PortInfo const &port) const
{
	view::SensorItem const *sensor = mView.sensorItem(port);
	QPointF const position = sensor ? sensor->scenePos() : QPointF();
	qreal const direction = sensor ? sensor->rotation() + mModel.robotModel().rotation() : 0;
	return { position, direction };
}
