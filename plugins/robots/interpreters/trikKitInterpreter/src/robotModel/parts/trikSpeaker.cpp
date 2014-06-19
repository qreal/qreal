#include "trikSpeaker.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikSpeaker::TrikSpeaker(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Speaker(info, port)
{
}
