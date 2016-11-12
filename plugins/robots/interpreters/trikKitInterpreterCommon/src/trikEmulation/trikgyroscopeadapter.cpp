#include <trikKitInterpreterCommon/trikEmulation/trikgyroscopeadapter.h>

#include <twoDModel/engine/twoDModelEngineInterface.h>

TrikGyroscopeAdapter::TrikGyroscopeAdapter(const kitBase::robotModel::PortInfo &port, twoDModel::engine::TwoDModelEngineInterface *engine)
	: mPort(port), mEngine(engine)
{
}

QVector<int> TrikGyroscopeAdapter::read() const
{
	return mEngine->readGyroscopeSensor(); // replace with QMetaObject::invokeMethod call?
}
