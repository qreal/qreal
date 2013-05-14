#include "unrealEncoderImplementation.h"
#include <QtCore/QDebug>
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealEncoderImplementation::UnrealEncoderImplementation(const outputPort::OutputPortEnum &port, d2Model::D2RobotModel *d2Model)
	: AbstractEncoderImplementation(port)
{
	mD2Model = d2Model;
}

void UnrealEncoderImplementation::read()
{
	emit response(mD2Model->readEncoder(mPort));
}

void UnrealEncoderImplementation::nullificate()
{
	mD2Model->resetEncoder(mPort);
}
