#include <trikKitInterpreterCommon/trikEmulation/displayemu.h>

#include <kitBase/robotModel/robotModelUtils.h>
///todo:
#include <trikKitInterpreterCommon/robotModel/twoD/parts/twoDDisplay.h>

using namespace trik;

TrikDisplayEmu::TrikDisplayEmu(QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> model) : mTwoDRobotModel(model), mDisplay(nullptr)
{
}

trikControl::DisplayWidgetInterface &TrikDisplayEmu::graphicsWidget() {
	trikControl::DisplayWidgetInterface temp;
	return temp; // bad
}

void TrikDisplayEmu::init()
{
	mDisplay = kitBase::robotModel::RobotModelUtils::findDevice<robotModel::parts::TrikDisplay>(*mTwoDRobotModel, "DisplayPort");
}

void TrikDisplayEmu::showImage(const QString &fileName)
{
//		trik::robotModel::parts::TrikDisplay * const display =
//				kitBase::robotModel::RobotModelUtils::findDevice<trik::robotModel::parts::TrikDisplay>(*mTwoDRobotModel, "DisplayPort");
		const bool smile = fileName.endsWith(QLatin1String("sad.png"));
		bool res = QMetaObject::invokeMethod(mDisplay,
		                                     "drawSmile",
		                                     Q_ARG(bool, smile));
}

void TrikDisplayEmu::setPainterColor(const QString &color)
{
	QMetaObject::invokeMethod(mDisplay,
	                          "setPainterColor",
	                          Q_ARG(const QColor &, QColor(color)));
}

void TrikDisplayEmu::setPainterWidth(int penWidth)
{
	QMetaObject::invokeMethod(mDisplay,
	                          "setPainterWidth",
	                          Q_ARG(int, penWidth));
}

void TrikDisplayEmu::drawLine(int x1, int y1, int x2, int y2)
{
	QMetaObject::invokeMethod(mDisplay,
	                          "drawArc",
	                          Q_ARG(int, x1),
	                          Q_ARG(int, y1),
	                          Q_ARG(int, x2),
	                          Q_ARG(int, y2));
}

void TrikDisplayEmu::drawPoint(int x, int y)
{
	QMetaObject::invokeMethod(mDisplay,
	                          "drawPixel",
	                          Q_ARG(int, x),
	                          Q_ARG(int, y));
}

void TrikDisplayEmu::drawRect(int x, int y, int width, int height)
{
	QMetaObject::invokeMethod(mDisplay,
	                          "drawRect",
	                          Q_ARG(int, x),
	                          Q_ARG(int, y),
	                          Q_ARG(int, width),
	                          Q_ARG(int, height));
}

void TrikDisplayEmu::drawEllipse(int x, int y, int width, int height)
{
	QMetaObject::invokeMethod(mDisplay,
	                          "drawEllipse",
	                          Q_ARG(int, x),
	                          Q_ARG(int, y),
	                          Q_ARG(int, width),
	                          Q_ARG(int, height));
}

void TrikDisplayEmu::drawArc(int x, int y, int width, int height, int startAngle, int spanAngle)
{
	QMetaObject::invokeMethod(mDisplay,
	                          "drawArc",
	                          Q_ARG(int, x),
	                          Q_ARG(int, y),
	                          Q_ARG(int, width),
	                          Q_ARG(int, height),
	                          Q_ARG(int, startAngle),
	                          Q_ARG(int, spanAngle));
}

void TrikDisplayEmu::setBackground(const QString &color)
{
	QMetaObject::invokeMethod(mDisplay,
	                          "setBackground",
	                          Q_ARG(const QColor &, QColor(color)));
}

void TrikDisplayEmu::clear()
{
	QMetaObject::invokeMethod(mDisplay,
	                          "clearScreen");
}

void TrikDisplayEmu::redraw()
{
	QMetaObject::invokeMethod(mDisplay,
	                          "redraw");
}
