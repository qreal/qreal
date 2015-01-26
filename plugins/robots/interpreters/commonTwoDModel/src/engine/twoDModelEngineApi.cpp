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

TwoDModelEngineApi::TwoDModelEngineApi(model::Model &model, view::D2ModelWidget &view)
	: mModel(model)
	, mView(view)
{
}

void TwoDModelEngineApi::setNewMotor(int speed, uint degrees, PortInfo const &port, bool breakMode)
{
	mModel.robotModels()[0]->setNewMotor(speed, degrees, port, breakMode);
}

int TwoDModelEngineApi::readEncoder(PortInfo const &port) const
{
	return mModel.robotModels()[0]->readEncoder(port);
}

void TwoDModelEngineApi::resetEncoder(PortInfo const &port)
{
	mModel.robotModels()[0]->resetEncoder(port);
}

int TwoDModelEngineApi::readTouchSensor(PortInfo const &port) const
{
	if (!mModel.robotModels()[0]->configuration().type(port).isA<robotParts::TouchSensor>()) {
		return touchSensorNotPressedSignal;
	}

	QPair<QPointF, qreal> const neededPosDir = countPositionAndDirection(port);
	QPointF const position(neededPosDir.first);
	qreal const rotation = neededPosDir.second / 180 * mathUtils::pi;
	QSizeF const size = mModel.robotModels()[0]->sensorRect(port, position).size();

	QPainterPath sensorPath;
	qreal const touchRegionRadius = size.height() / 2;
	qreal const stickCenter = size.width() / 2 - touchRegionRadius;
	// (0,0) in sensor coordinates is sensor`s center
	QPointF const ellipseCenter = QPointF(stickCenter * cos(rotation), stickCenter * sin(rotation));
	sensorPath.addEllipse(position + ellipseCenter, touchRegionRadius, touchRegionRadius);

	bool const pressed = mModel.worldModel().checkCollision(sensorPath);
	return pressed ? touchSensorPressedSignal : touchSensorNotPressedSignal;
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

int TwoDModelEngineApi::readColorSensor(PortInfo const &port) const
{
	QImage const image = printColorSensor(port);
	QHash<uint, int> countsColor;

	uint const *data = reinterpret_cast<uint const *>(image.bits());
	int const n = image.byteCount() / 4;
	for (int i = 0; i < n; ++i) {
		uint const color = mModel.settings().realisticSensors() ? spoilColor(data[i]) : data[i];
		++countsColor[color];
	}

	if (mModel.robotModels()[0]->configuration().type(port).isA<robotParts::ColorSensorFull>()) {
		return readColorFullSensor(countsColor);
	} else if (mModel.robotModels()[0]->configuration().type(port).isA<robotParts::ColorSensorPassive>()) {
		return readColorNoneSensor(countsColor, n);
	} else if (mModel.robotModels()[0]->configuration().type(port).isA<robotParts::ColorSensorRed>()) {
		return readSingleColorSensor(red, countsColor, n);
	} else if (mModel.robotModels()[0]->configuration().type(port).isA<robotParts::ColorSensorGreen>()) {
		return readSingleColorSensor(green, countsColor, n);
	} else if (mModel.robotModels()[0]->configuration().type(port).isA<robotParts::ColorSensorBlue>()) {
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

QImage TwoDModelEngineApi::printColorSensor(PortInfo const &port) const
{
	DeviceInfo const device = mModel.robotModels()[0]->configuration().type(port);
	if (device.isNull()) {
		return QImage();
	}

	QPair<QPointF, qreal> const neededPosDir = countPositionAndDirection(port);
	QPointF const position = neededPosDir.first;
	qreal const width = mModel.robotModels()[0]->info().sensorImageRect(device).width() / 2.0;
	QRectF const scanningRect = QRectF(position.x() - width, position.y() - width, 2 * width, 2 * width);

	QImage image(scanningRect.size().toSize(), QImage::Format_RGB32);
	QPainter painter(&image);

	QBrush brush(Qt::SolidPattern);
	brush.setColor(Qt::white);
	painter.setBrush(brush);
	painter.setPen(QPen(Qt::white));
	painter.drawRect(scanningRect.translated(-scanningRect.topLeft()));

	QGraphicsItem * const sensorItem = mView.sensorItem(port);
	view::RobotItem * const robot = dynamic_cast<view::RobotItem *>(mView.sensorItem(port)->parentItem());
	bool const wasSelected = sensorItem->isSelected();
	bool const rotaterWasVisible = robot->rotater()->isVisible();
	bool const rotaterWasSelected = robot->rotater()->isSelected();
	mView.setSensorVisible(port, false);
	robot->rotater()->setVisible(false);

	mView.scene()->render(&painter, QRectF(), scanningRect);

	mView.setSensorVisible(port, true);
	mView.sensorItem(port)->setSelected(wasSelected);
	robot->rotater()->setVisible(rotaterWasVisible);
	robot->rotater()->setSelected(rotaterWasSelected);

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

int TwoDModelEngineApi::readLightSensor(PortInfo const &port) const
{
	// Must return 1023 on white and 0 on black normalized to percents
	// http://stackoverflow.com/questions/596216/formula-to-determine-brightness-of-rgb-color

	QImage const image = printColorSensor(port);
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
	mModel.robotModels()[0]->playSound(timeInMs);
}

void TwoDModelEngineApi::markerDown(QColor const &color)
{
	mModel.robotModels()[0]->markerDown(color);
}

void TwoDModelEngineApi::markerUp()
{
	mModel.robotModels()[0]->markerUp();
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
	qreal const direction = sensor ? sensor->rotation() + mModel.robotModels()[0]->rotation() : 0;
	return { position, direction };
}
