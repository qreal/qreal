#include "twoDModelEngineApi.h"

#include <QtCore/QDebug>
#include <QtCore/qmath.h>

#include <qrkernel/settingsManager.h>
#include <qrutils/mathUtils/math.h>
/// @todo: Get rid of it!
#include <kitBase/robotModel/robotParts/touchSensor.h>
#include <kitBase/robotModel/robotParts/colorSensorFull.h>
#include <kitBase/robotModel/robotParts/colorSensorPassive.h>
#include <kitBase/robotModel/robotParts/colorSensorRed.h>
#include <kitBase/robotModel/robotParts/colorSensorGreen.h>
#include <kitBase/robotModel/robotParts/colorSensorBlue.h>

#include "model/model.h"
#include "model/constants.h"

#include "twoDModel/engine/view/d2ModelWidget.h"
#include "view/d2ModelScene.h"
#include "view/robotItem.h"

using namespace twoDModel;
using namespace kitBase::robotModel;
using namespace twoDModel::model;

TwoDModelEngineApi::TwoDModelEngineApi(model::Model &model, view::D2ModelWidget &view)
	: mModel(model)
	, mView(view)
{
}

void TwoDModelEngineApi::setNewMotor(int speed, uint degrees, const PortInfo &port, bool breakMode)
{
	mModel.robotModels()[0]->setNewMotor(speed, degrees, port, breakMode);
}

int TwoDModelEngineApi::readEncoder(const PortInfo &port) const
{
	return mModel.robotModels()[0]->readEncoder(port);
}

void TwoDModelEngineApi::resetEncoder(const PortInfo &port)
{
	mModel.robotModels()[0]->resetEncoder(port);
}

int TwoDModelEngineApi::readTouchSensor(const PortInfo &port) const
{
	if (!mModel.robotModels()[0]->configuration().type(port).isA<robotParts::TouchSensor>()) {
		return touchSensorNotPressedSignal;
	}

	QPair<QPointF, qreal> const neededPosDir = countPositionAndDirection(port);
	const QPointF position(neededPosDir.first);
	const qreal rotation = neededPosDir.second / 180 * mathUtils::pi;
	const QSizeF size = mModel.robotModels()[0]->sensorRect(port, position).size();

	QPainterPath sensorPath;
	const qreal touchRegionRadius = size.height() / 2;
	const qreal stickCenter = size.width() / 2 - touchRegionRadius;
	// (0,0) in sensor coordinates is sensor`s center
	const QPointF ellipseCenter = QPointF(stickCenter * cos(rotation), stickCenter * sin(rotation));
	sensorPath.addEllipse(position + ellipseCenter, touchRegionRadius, touchRegionRadius);

	const bool pressed = mModel.worldModel().checkCollision(sensorPath);
	return pressed ? touchSensorPressedSignal : touchSensorNotPressedSignal;
}

int TwoDModelEngineApi::readSonarSensor(const PortInfo &port) const
{
	QPair<QPointF, qreal> neededPosDir = countPositionAndDirection(port);
	const int res = mModel.worldModel().sonarReading(neededPosDir.first, neededPosDir.second);

	return mModel.settings().realisticSensors() ? spoilSonarReading(res) : res;
}

int TwoDModelEngineApi::spoilSonarReading(const int distance) const
{
	const qreal ran = mathUtils::Math::gaussianNoise(spoilSonarDispersion);
	return mathUtils::Math::truncateToInterval(0, 255, round(distance + ran));
}

int TwoDModelEngineApi::readColorSensor(const PortInfo &port) const
{
	const QImage image = areaUnderSensor(port, 1.0);
	QHash<uint, int> countsColor;

	const uint *data = reinterpret_cast<const uint *>(image.bits());
	const int n = image.byteCount() / 4;
	for (int i = 0; i < n; ++i) {
		const uint color = mModel.settings().realisticSensors() ? spoilColor(data[i]) : data[i];
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

uint TwoDModelEngineApi::spoilColor(const uint color) const
{
	const qreal noise = mathUtils::Math::gaussianNoise(spoilColorDispersion);

	int r = round(((color >> 16) & 0xFF) + noise);
	int g = round(((color >> 8) & 0xFF) + noise);
	int b = round(((color >> 0) & 0xFF) + noise);
	const int a = (color >> 24) & 0xFF;

	r = mathUtils::Math::truncateToInterval(0, 255, r);
	g = mathUtils::Math::truncateToInterval(0, 255, g);
	b = mathUtils::Math::truncateToInterval(0, 255, b);

	return ((r & 0xFF) << 16) + ((g & 0xFF) << 8) + (b & 0xFF) + ((a & 0xFF) << 24);
}

QImage TwoDModelEngineApi::areaUnderSensor(const PortInfo &port, qreal widthFactor) const
{
	DeviceInfo device = mModel.robotModels()[0]->configuration().type(port);
	if (device.isNull()) {
		device = mModel.robotModels()[0]->info().specialDevice(port);
		if (device.isNull()) {
			return QImage();
		}
	}

	QGraphicsItem * const sensorItem = mView.sensorItem(port);
	const QPair<QPointF, qreal> neededPosDir = countPositionAndDirection(port);
	const QPointF position = neededPosDir.first;
	const qreal direction = neededPosDir.second;
	const QRect imageRect = mModel.robotModels()[0]->info().sensorImageRect(device);
	const qreal width = imageRect.width() * widthFactor / 2.0;
	const qreal rotationFactor = sensorItem->mapToScene(imageRect).boundingRect().width() / imageRect.width();
	const qreal realWidth = width * rotationFactor;
	const QRectF scanningRect = QRectF(position.x() - realWidth, position.y() - realWidth
			, 2 * realWidth, 2 * realWidth);

	QImage image(scanningRect.size().toSize(), QImage::Format_RGB32);
	QPainter painter(&image);

	QBrush brush(Qt::SolidPattern);
	brush.setColor(Qt::white);
	painter.setBrush(brush);
	painter.setPen(QPen(Qt::white));
	painter.drawRect(scanningRect.translated(-scanningRect.topLeft()));

	view::RobotItem * const robot = dynamic_cast<view::RobotItem *>(mView.sensorItem(port)->parentItem());
	const bool wasSelected = sensorItem->isSelected();
	const bool rotaterWasVisible = robot->rotater().isVisible();
	const bool rotaterWasSelected = robot->rotater().isSelected();
	robot->setVisible(false);

	mView.scene()->render(&painter, QRectF(), scanningRect);

	mView.sensorItem(port)->setSelected(wasSelected);
	robot->setVisible(true);
	robot->rotater().setVisible(rotaterWasVisible);
	robot->rotater().setSelected(rotaterWasSelected);

	const QPoint offset = QPointF(width, width).toPoint();
	const QImage rotated(image.transformed(QTransform().rotate(-(90 + direction))));
	const QRect realImage(rotated.rect().center() - offset, rotated.rect().center() + offset - QPoint(1, 1));
	return rotated.copy(realImage);
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

	const uint maxColor = countsColor.key(maxValue);
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
		const uint color = i.key();
		if (color != white) {
			const int b = (color >> 0) & 0xFF;
			const int g = (color >> 8) & 0xFF;
			const int r = (color >> 16) & 0xFF;
			const qreal k = qSqrt(static_cast<qreal>(b * b + g * g + r * r)) / 500.0;
			allWhite += static_cast<qreal>(i.value()) * k;
		}
	}

	return (allWhite / static_cast<qreal>(n)) * 100.0;
}

int TwoDModelEngineApi::readLightSensor(const PortInfo &port) const
{
	// Must return 1023 on white and 0 on black normalized to percents
	// http://stackoverflow.com/questions/596216/formula-to-determine-brightness-of-rgb-color

	const QImage image = areaUnderSensor(port, 1.0);
	if (image.isNull()) {
		return 0;
	}

	uint sum = 0;
	const uint *data = reinterpret_cast<const uint *>(image.bits());
	const int n = image.byteCount() / 4;

	for (int i = 0; i < n; ++i) {
		const int color = mModel.settings().realisticSensors() ? spoilLight(data[i]) : data[i];
		const int b = (color >> 0) & 0xFF;
		const int g = (color >> 8) & 0xFF;
		const int r = (color >> 16) & 0xFF;
		// brightness in [0..256]
		const int brightness = 0.2126 * r + 0.7152 * g + 0.0722 * b;

		sum += 4 * brightness; // 4 = max sensor value / max brightness value
	}
	const qreal rawValue = sum / n; // Average by whole region
	return rawValue * 100 / maxLightSensorValur; // Normalizing to percents
}

void TwoDModelEngineApi::playSound(int timeInMs)
{
	mModel.robotModels()[0]->playSound(timeInMs);
}

void TwoDModelEngineApi::markerDown(const QColor &color)
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

uint TwoDModelEngineApi::spoilLight(const uint color) const
{
	const qreal noise = mathUtils::Math::gaussianNoise(spoilLightDispersion);

	if (noise > (1.0 - percentSaltPepperNoise / 100.0)) {
		return white;
	} else if (noise < (-1.0 + percentSaltPepperNoise / 100.0)) {
		return black;
	}

	return color;
}

QPair<QPointF, qreal> TwoDModelEngineApi::countPositionAndDirection(const PortInfo &port) const
{
	const view::SensorItem *sensor = mView.sensorItem(port);
	const QPointF position = sensor ? sensor->scenePos() : QPointF();
	const qreal direction = sensor ? sensor->rotation() + mModel.robotModels()[0]->rotation() : 0;
	return { position, direction };
}
