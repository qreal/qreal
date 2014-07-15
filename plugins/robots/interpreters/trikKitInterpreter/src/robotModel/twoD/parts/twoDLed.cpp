#include "twoDLed.h"
#include "trikDisplayWidget.h"

using namespace trikKitInterpreter::robotModel::twoD::parts;
using namespace interpreterBase::robotModel;


TwoDLed::TwoDLed(DeviceInfo const &info
		, PortInfo const &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikLed(info, port)
	, mEngine(engine)
{

}

///set Color in Led on display
void TwoDLed::setColor(QColor const &color)
{
	auto display = dynamic_cast<TrikDisplayWidget *>(mEngine.display());
	display->setLedColor(color);
}




