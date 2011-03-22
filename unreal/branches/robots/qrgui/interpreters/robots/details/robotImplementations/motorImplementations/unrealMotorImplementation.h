#pragma once
#include <QtCore/QObject>
#include "../../robotCommandConstants.h"
#include "abstractMotorImplementation.h"
#include "../../d2RobotModel/d2RobotModel.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace motorImplementations {

class UnrealMotorImplementation : public AbstractMotorImplementation
{
	Q_OBJECT
public:
	UnrealMotorImplementation(int const port, d2Model::D2RobotModel *d2Model);
	virtual void on(int speed);
	virtual void on(int speed, long unsigned int degrees);
	virtual void stop();
	virtual void off();
	virtual void resetMotorPosition(bool relative);
private:
	d2Model::D2RobotModel *mD2Model;
};

}
}
}
}
}
}
