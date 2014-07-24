#include "trikCameraLineDetector.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikLineSensor::TrikLineSensor(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::ScalarSensor(info, port)
{
}
