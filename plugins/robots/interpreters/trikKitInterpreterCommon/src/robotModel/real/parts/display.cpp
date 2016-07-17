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

#include "trikKitInterpreterCommon/robotModel/real/parts/display.h"

#include <QtGui/QColor>

#include <qrutils/inFile.h>

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

Display::Display(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikDisplay(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void Display::drawSmile(bool sad)
{
	const QString pathToCommand = sad ? ":/trikQts/templates/drawing/sadSmile.t"
			: ":/trikQts/templates/drawing/smile.t";

	const QString directCommand = utils::InFile::readAll(pathToCommand);
	mRobotCommunicator.runDirectCommand(directCommand);
}

void Display::setBackground(const QColor &color)
{
	const QString pathToCommand = ":/trikQts/templates/drawing/setBackground.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@COLOR@@", color.name()).replace("@@REDRAW@@", QString());

	mRobotCommunicator.runDirectCommand(directCommand);
}

void Display::clearScreen()
{
	const QString pathToCommand = ":/trikQts/templates/drawing/clearScreen.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand).replace("@@REDRAW@@", QString());
	mRobotCommunicator.runDirectCommand(directCommand);
}

void Display::printText(int x, int y, const QString &text)
{
	const QString pathToCommand = ":/trikQts/templates/drawing/printText.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand).replace("@@TEXT@@", "\"" + text + "\"")
			.replace("@@X@@", QString::number(x)).replace("@@Y@@", QString::number(y))
			.replace("@@REDRAW@@", QString());

	mRobotCommunicator.runDirectCommand(directCommand);
}

void Display::setPainterColor(const QColor &color)
{
	Q_UNUSED(color)
}

void Display::setPainterWidth(int penWidth)
{
	Q_UNUSED(penWidth)
}

void Display::drawPixel(int x, int y)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
}

void Display::drawLine(int x1, int y1, int x2, int y2)
{
	Q_UNUSED(x1)
	Q_UNUSED(y1)
	Q_UNUSED(x2)
	Q_UNUSED(y2)
}

void Display::drawRect(int x, int y, int width, int height)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(width)
	Q_UNUSED(height)
}

void Display::drawEllipse(int x, int y, int width, int height)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(width)
	Q_UNUSED(height)
}

void Display::drawArc(int x, int y, int width, int height, int startAngle, int spanAngle)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(width)
	Q_UNUSED(height)
	Q_UNUSED(startAngle)
	Q_UNUSED(spanAngle)
}

void Display::redraw()
{
	const QString pathToCommand = ":/trikQts/templates/drawing/redraw.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand);
	mRobotCommunicator.runDirectCommand(directCommand);
}
