#pragma once

#include <QtCore/QPointF>
#include <QtCore/QSizeF>

namespace twoDModel {

qreal const lowPrecision = 0.00001;

qreal const robotWidth = 50;
qreal const robotHeight = 50;
const QPointF rotatePoint = QPointF(robotWidth / 2, robotHeight / 2);
QSize const displaySize(200, 300);
qreal const beepWavesSize = 120;

qreal const robotWheelDiameterInPx = 16;
qreal const robotWheelDiameterInCm = 5.6;
qreal const pixelsInCm = robotWheelDiameterInPx / robotWheelDiameterInCm;

const uint black   = 0xFF000000;
const uint white   = 0xFFFFFFFF;
const uint red     = 0xFFFF0000;
const uint green   = 0xFF008000;
const uint blue    = 0xFF0000FF;
const uint yellow  = 0xFFFFFF00;
const uint cyan    = 0xFF00FFFF;
const uint magenta = 0xFFFF00FF;

qreal const spoilColorDispersion = 2.0;
qreal const spoilLightDispersion = 1.0;
qreal const spoilSonarDispersion = 1.5;
qreal const varySpeedDispersion = 0.0125;
qreal const percentSaltPepperNoise = 20.0;

qreal const robotMass = 800;
qreal const robotInertialMoment = 20;
qreal const reactionForceStabilizationCoefficient = 20;
qreal const floorFrictionCoefficient = 0.2;
qreal const wallFrictionCoefficient = 0.2;
qreal const rotationalFrictionFactor = 1500;
qreal const angularVelocityFrictionFactor = 200;

qreal const onePercentAngularVelocity = 0.0055;
const int maxLightSensorValur = 1023;
const int touchSensorPressedSignal = 1;
const int touchSensorNotPressedSignal = 0;

}
