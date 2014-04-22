#include "trikCameraLineDetector.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikCameraLineDetector::TrikCameraLineDetector(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::ScalarSensor(info, port)
{
}

void TrikCameraLineDetector::doConfiguration()
{
	configurationCompleted(true);
}
