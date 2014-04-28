#include "trikTwoDModelConfigurer.h"

using namespace trikKitInterpreter;
using namespace interpreterBase::robotModel;

TrikTwoDModelConfigurer::TrikTwoDModelConfigurer(QString const &leftWheelPort, QString const &rightWheelPort)
	: mLeftWheelPort(leftWheelPort)
	, mRightWheelPort(rightWheelPort)
{
}

QString TrikTwoDModelConfigurer::robotImage() const
{
	return ":icons/trikTwoDRobot.svg";
}

PortInfo TrikTwoDModelConfigurer::defaultLeftWheelPort() const
{
	return PortInfo(mLeftWheelPort, output);
}

PortInfo TrikTwoDModelConfigurer::defaultRightWheelPort() const
{
	return PortInfo(mRightWheelPort, output);
}
