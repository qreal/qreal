#include <trikKitInterpreterCommon/trikEmulation/trikencoderadapter.h>


trik::TrikEncoderAdapter::TrikEncoderAdapter(kitBase::robotModel::robotParts::EncoderSensor *encoder)
	: mEncoder(encoder)
{
}

int trik::TrikEncoderAdapter::read()
{
	QMetaObject::invokeMethod(mEncoder, "read");
	return mEncoder->lastData();
}

void trik::TrikEncoderAdapter::reset()
{
	mEncoder->nullify();
}
