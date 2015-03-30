#include "trikKit/robotModel/parts/trikSpeaker.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikSpeaker::TrikSpeaker(const DeviceInfo &info, const PortInfo &port)
	: kitBase::robotModel::robotParts::Speaker(info, port)
{
}
