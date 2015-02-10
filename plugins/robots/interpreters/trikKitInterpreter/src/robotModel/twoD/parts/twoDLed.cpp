#include "twoDLed.h"
#include "trikDisplayWidget.h"

using namespace trikKitInterpreter::robotModel::twoD::parts;
using namespace interpreterBase::robotModel;

TwoDLed::TwoDLed(const DeviceInfo &info
		, const PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikLed(info, port)
	, mEngine(engine)
{
}

void TwoDLed::setColor(const QString &color)
{
	const QColor newColor = color == "off" ? QColor(Qt::gray) : QColor(color);
	auto display = dynamic_cast<TrikDisplayWidget *>(mEngine.display());
	Q_ASSERT(display);
	display->setLedColor(newColor);
}
