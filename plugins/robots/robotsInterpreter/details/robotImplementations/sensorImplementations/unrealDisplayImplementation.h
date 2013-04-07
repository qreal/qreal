#pragma once

#include "abstractDisplayImplementation.h"
#include "../../robotCommandConstants.h"
#include "../../d2RobotModel/d2RobotModel.h"
#include "../details/nxtDisplay.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {
class UnrealDisplayImplementation : public AbstractDisplayImplementation
{
	Q_OBJECT
public:
	UnrealDisplayImplementation(d2Model::D2RobotModel *d2Model);
	virtual void read();

protected:
	d2Model::D2RobotModel *mD2Model;
};
}
}
}
}
}
}
