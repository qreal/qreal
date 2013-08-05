#pragma once
#include "abstractEncoderImplementation.h"
#include "../../d2RobotModel/d2RobotModel.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class UnrealEncoderImplementation : public AbstractEncoderImplementation
{
	Q_OBJECT
public:
	UnrealEncoderImplementation(enums::outputPort::OutputPortEnum const &port, d2Model::D2RobotModel *d2Model);
	virtual void read();
	virtual void nullificate();

protected:
d2Model::D2RobotModel *mD2Model;
};

}
}
}
}
}
}
