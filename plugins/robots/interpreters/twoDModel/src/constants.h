#pragma once

#include <QtCore/QPointF>
#include <QtCore/QSizeF>

namespace twoDModel {

qreal const lowPrecision = 0.00001;

qreal const robotWidth = 50;
qreal const robotHeight = 50;
QPointF const rotatePoint = QPointF(robotWidth / 2, robotHeight / 2);
QSize const displaySize(200, 300);
qreal const beepWavesSize = 120;

uint const black   = 0xFF000000;
uint const white   = 0xFFFFFFFF;
uint const red     = 0xFFFF0000;
uint const green   = 0xFF008000;
uint const blue    = 0xFF0000FF;
uint const yellow  = 0xFFFFFF00;
uint const cyan    = 0xFF00FFFF;
uint const magenta = 0xFFFF00FF;

qreal const spoilColorDispersion = 2.0;
qreal const spoilLightDispersion = 1.0;
qreal const spoilSonarDispersion = 1.5;
qreal const varySpeedDispersion = 0.0125;
qreal const percentSaltPepperNoise = 20.0;

qreal const robotMass = 200;
qreal const robotInertialMoment = 20;
qreal const reactionForceStabilizationCoefficient = 20;
qreal const floorFrictionCoefficient = 0.2;
qreal const wallFrictionCoefficient = 0.2;
qreal const rotationalFrictionFactor = 1500;
qreal const angularVelocityFrictionFactor = 200;

qreal const onePercentAngularVelocity = 0.0055;
int const touchSensorWallStrokeIncrement = 10;
int const touchSensorStrokeIncrement = 5;
int const maxLightSensorValur = 1023;
int const touchSensorPressedSignal = 1;
int const touchSensorNotPressedSignal = 0;

}
