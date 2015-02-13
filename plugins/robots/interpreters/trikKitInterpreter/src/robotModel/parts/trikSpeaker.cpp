#include "trikSpeaker.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikSpeaker::TrikSpeaker(const DeviceInfo &info, const PortInfo &port)
	: interpreterBase::robotModel::robotParts::Speaker(info, port)
{
}
