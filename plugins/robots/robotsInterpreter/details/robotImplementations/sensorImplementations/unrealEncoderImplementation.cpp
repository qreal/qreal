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
        int readValue = mD2Model->readEncoder(mPort);
        emit response(readValue);
}

void UnrealEncoderImplementation::nullificate()
{
        mD2Model->resetEncoder(mPort);
}
