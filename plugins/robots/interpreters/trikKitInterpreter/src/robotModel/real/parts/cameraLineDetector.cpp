#include "cameraLineDetector.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

CameraLineDetector::CameraLineDetector(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikCameraLineDetector(info, port)
{
}

void CameraLineDetector::init()
{
}

void CameraLineDetector::detectLine()
{
}

void CameraLineDetector::read()
{
	emit newData(0);
}
