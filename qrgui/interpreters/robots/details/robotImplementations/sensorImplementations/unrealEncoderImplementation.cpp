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
	emit response(0); //TODO: сделать поддержку в 2D-модели
}

void UnrealEncoderImplementation::nullificate()
{
	//TODO: сделать поддержку в 2D-модели
}
