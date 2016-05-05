/* Copyright 2015 CyberTech Labs Ltd.
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

#include "trikKitInterpreterCommon/robotModel/twoD/trikThreeDRobotModel.h"

#include <QtGui/QColor>

#include <qrkernel/logging.h>

#include <kitBase/robotModel/robotModelUtils.h>
#include <kitBase/robotModel/robotParts/lightSensor.h>
#include <kitBase/robotModel/robotParts/touchSensor.h>

#include <trikKit/robotModel/parts/trikLineSensor.h>
#include <trikKit/robotModel/parts/trikObjectSensor.h>
#include <trikKit/robotModel/parts/trikColorSensor.h>
#include <trikKit/robotModel/parts/trikInfraredSensor.h>
#include <trikKit/robotModel/parts/trikSonarSensor.h>
#include <trikKit/robotModel/parts/trikShell.h>
#include <trikKitInterpreterCommon/trikDisplayWidget.h>

#include "trikKitInterpreterCommon/robotModel/twoD/parts/twoDDisplay.h"
#include "trikKitInterpreterCommon/robotModel/twoD/parts/twoDSpeaker.h"
#include "trikKitInterpreterCommon/robotModel/twoD/parts/twoDShell.h"
#include "trikKitInterpreterCommon/robotModel/twoD/parts/twoDInfraredSensor.h"
#include "trikKitInterpreterCommon/robotModel/twoD/parts/twoDLed.h"
#include "trikKitInterpreterCommon/robotModel/twoD/parts/twoDLineSensor.h"
#include "trikKitInterpreterCommon/robotModel/twoD/parts/twoDObjectSensor.h"
#include "trikKitInterpreterCommon/robotModel/twoD/parts/twoDColorSensor.h"

using namespace trik::robotModel;
using namespace trik::robotModel::twoD;
using namespace kitBase::robotModel;

TrikThreeDRobotModel::TrikThreeDRobotModel(RobotModelInterface &realModel)
	//: twoDModel::robotModel::ThreeDRobotModel(realModel)
	: twoDModel::robotModel::TwoDRobotModel(realModel)
	, mRealModel(&realModel)
	, mLeftWheelPort("M3")
	, mRightWheelPort("M4")
	, mDisplayWidget(new TrikDisplayWidget())
{
	/// @todo: One day we will support gamepad in 2D model and there will be piece.
	/// But till that day gamepad ports must be killed cause they spam logs.
	const QList<PortInfo> realRobotPorts = CommonRobotModel::availablePorts();
	for (const PortInfo &port : realRobotPorts) {
		if (port.name().contains("Gamepad", Qt::CaseInsensitive)) {
			removeAllowedConnections(port);
		}
	}
}

QString TrikThreeDRobotModel::friendlyName() const
{
	QRegExp versionRegExp("\\(v.*\\)");
	const int pos = versionRegExp.indexIn(mRealModel->friendlyName());
	if (pos == -1) {\
		return tr("3D модель");
		//return tr("3D Model");
	}

	return tr("3D Model") + " " + versionRegExp.capturedTexts().at(0);
}

robotParts::Device *TrikThreeDRobotModel::createDevice(const PortInfo &port, const DeviceInfo &deviceInfo)
{
	if (deviceInfo.isA<robotParts::Display>()) {
		return new parts::Display(deviceInfo, port, *engine());
	}

	if (deviceInfo.isA<robotParts::Speaker>()) {
		return new parts::TwoDSpeaker(deviceInfo, port, *engine());
	}

	if (deviceInfo.isA<robotModel::parts::TrikShell>()) {
		parts::Shell * const shell = new parts::Shell(deviceInfo, port, *engine());
		// Error reporter will come only after global plugin init() is called. Shell is however
		// configured even later. So setting error reporter only when everything will be ready.
		connect(shell, &parts::Shell::configured, [=]() { shell->setErrorReporter(*mErrorReporter); });
		return shell;
	}

	if (deviceInfo.isA<robotModel::parts::TrikInfraredSensor>()) {
		return new parts::TwoDInfraredSensor(deviceInfo, port, *engine());
	}

	if (deviceInfo.isA<robotModel::parts::TrikLed>()) {
		return new parts::TwoDLed(deviceInfo, port, *engine());
	}

	if (deviceInfo.isA<robotModel::parts::TrikLineSensor>()) {
		return new parts::LineSensor(deviceInfo, port, *engine());
	}

	if (deviceInfo.isA<robotModel::parts::TrikObjectSensor>()) {
		return new parts::ObjectSensor(deviceInfo, port);
	}

	if (deviceInfo.isA<robotModel::parts::TrikColorSensor>()) {
		return new parts::ColorSensor(deviceInfo, port);
	}

//	return twoDModel::robotModel::ThreeDRobotModel::createDevice(port, deviceInfo);
	return twoDModel::robotModel::TwoDRobotModel::createDevice(port, deviceInfo);
}

void TrikThreeDRobotModel::onInterpretationStarted()
{
	robotModel::parts::TrikDisplay * const display =
			RobotModelUtils::findDevice<robotModel::parts::TrikDisplay>(*this, "DisplayPort");
	if (display) {
		display->clearScreen();
		display->setBackground(QColor(Qt::gray));
		display->redraw();
	} else {
		QLOG_WARN() << "TRIK display is not configured before intepretation start!";
	}

	parts::Shell * const shell = RobotModelUtils::findDevice<parts::Shell>(*this, "ShellPort");
	if (shell) {
		shell->reset();
	} else {
		QLOG_WARN() << "TRIK shell is not configured before intepretation start!";
	}
}

QString TrikThreeDRobotModel::robotImage() const
{
	return ":icons/trikTwoDRobot.svg";
}

PortInfo TrikThreeDRobotModel::defaultLeftWheelPort() const
{
	return PortInfo(mLeftWheelPort, output);
}

PortInfo TrikThreeDRobotModel::defaultRightWheelPort() const
{
	return PortInfo(mRightWheelPort, output);
}

twoDModel::engine::TwoDModelDisplayWidget *TrikThreeDRobotModel::displayWidget() const
{
	return mDisplayWidget;
}

QString TrikThreeDRobotModel::sensorImagePath(const DeviceInfo &deviceType) const
{
	if (deviceType.isA<kitBase::robotModel::robotParts::LightSensor>()) {
		return ":icons/twoDColorEmpty.svg";
	} else if (deviceType.isA<robotModel::parts::TrikInfraredSensor>()) {
		return ":icons/twoDIrRangeSensor.svg";
	} else if (deviceType.isA<robotModel::parts::TrikSonarSensor>()) {
		return ":icons/twoDUsRangeSensor.svg";
	} else if (deviceType.isA<robotModel::parts::TrikLineSensor>()) {
		return ":icons/twoDVideoModule.svg";
	}

	return QString();
}

void TrikThreeDRobotModel::setWheelPorts(const QString &leftWheelPort, const QString &rightWheelPort)
{
	mLeftWheelPort = leftWheelPort;
	mRightWheelPort = rightWheelPort;
}

QRect TrikThreeDRobotModel::sensorImageRect(const kitBase::robotModel::DeviceInfo &deviceType) const
{
	if (deviceType.isA<robotParts::TouchSensor>()) {
			return QRect(-12, -5, 25, 10);
	} else if (deviceType.isA<robotParts::LightSensor>()) {
		return QRect(-6, -6, 12, 12);
	} else if (deviceType.isA<robotModel::parts::TrikInfraredSensor>()) {
		return QRect(-18, -18, 36, 36);
	} else if (deviceType.isA<robotModel::parts::TrikSonarSensor>()) {
		return QRect(-18, -18, 36, 36);
	} else if (deviceType.isA<robotModel::parts::TrikLineSensor>()) {
		return QRect(-9, -9, 18, 18);
	}

	return QRect();
}

QHash<kitBase::robotModel::PortInfo, kitBase::robotModel::DeviceInfo> TrikThreeDRobotModel::specialDevices() const
{
//	QHash<PortInfo, DeviceInfo> result(twoDModel::robotModel::ThreeDRobotModel::specialDevices());
	QHash<PortInfo, DeviceInfo> result(twoDModel::robotModel::TwoDRobotModel::specialDevices());
	return result;
}

QPair<QPoint, qreal> TrikThreeDRobotModel::specialDeviceConfiguration(const PortInfo &port) const
{
	if (port == PortInfo("LineSensorPort", input)) {
		return qMakePair(QPoint(1, 0), 0);
	}

//	return twoDModel::robotModel::ThreeDRobotModel::specialDeviceConfiguration(port);
	return twoDModel::robotModel::TwoDRobotModel::specialDeviceConfiguration(port);
}

QHash<QString, int> TrikThreeDRobotModel::buttonCodes() const
{
	QHash<QString, int> result;
	result["LeftButton"] = 105;
	result["RightButton"] = 106;
	result["UpButton"] = 103;
	result["DownButton"] = 108;
	result["EnterButton"] = 28;
	result["PowerButton"] = 116;
	result["EscButton"] = 1;
	return result;
}

void TrikThreeDRobotModel::setErrorReporter(qReal::ErrorReporterInterface &errorReporter)
{
	mErrorReporter = &errorReporter;
}
