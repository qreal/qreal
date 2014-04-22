#include "speaker.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Speaker::Speaker(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikSpeaker(info, port)
{
}
