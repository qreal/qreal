#include "twoDLed.h"
#include "trikDisplayWidget.h"

#include <QDebug>

using namespace trikKitInterpreter::robotModel::twoD::parts;
using namespace interpreterBase::robotModel;


TwoDLed::TwoDLed(DeviceInfo const &info
		, PortInfo const &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikLed(info, port)
	, mEngine(engine)
{

}

void TwoDLed::setColor(QColor const &color)
{
	qDebug() << "widget run";
	//it dosen't work, becouse LedBlock is empty
	auto display = dynamic_cast<TrikDisplayWidget *>(mEngine.display());
	display->setLedColor(color);
}




