#include "display.h"

#include <QtGui/QColor>

#include <qrutils/inFile.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Display::Display(const DeviceInfo &info, const PortInfo &port, utils::TcpRobotCommunicator &tcpRobotCommunicator)
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
			.replace("@@COLOR@@", color.name());

	mRobotCommunicator.runDirectCommand(directCommand);
}

void Display::clearScreen()
{
	const QString pathToCommand = ":/trikQts/templates/drawing/clearScreen.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand);
	mRobotCommunicator.runDirectCommand(directCommand);
}

void Display::printText(int x, int y, const QString &text)
{
	const QString pathToCommand = ":/trikQts/templates/drawing/printText.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand).replace("@@TEXT@@", "\"" + text + "\"")
			.replace("@@X@@", QString::number(x)).replace("@@Y@@", QString::number(y)) + "brick.run();";

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
