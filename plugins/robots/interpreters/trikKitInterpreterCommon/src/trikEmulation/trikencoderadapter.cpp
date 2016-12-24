#include <trikKitInterpreterCommon/trikEmulation/trikencoderadapter.h>

#include <twoDModel/engine/twoDModelEngineInterface.h>

#include <QtCore/QEventLoop>

trik::TrikEncoderAdapter::TrikEncoderAdapter(const kitBase::robotModel::PortInfo &port
											 , twoDModel::engine::TwoDModelEngineInterface *engine)
	: mPort(port), mEngine(engine)
{
}

int trik::TrikEncoderAdapter::read()
{
	return mEngine->readEncoder(mPort); //replace with QObject::invokeMethod later? (for thread safety)
}

void trik::TrikEncoderAdapter::reset()
{
	mEngine->resetEncoder(mPort);
}
