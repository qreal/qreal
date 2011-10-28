#pragma once
#include <QtCore/QObject>
#include "abstractBrickImplementation.h"
#include "../../d2RobotModel/d2RobotModel.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace brickImplementations {

class UnrealBrickImplementation : public AbstractBrickImplementation
{
	Q_OBJECT
public:
	UnrealBrickImplementation(d2Model::D2RobotModel *d2Model);
	virtual void beep(unsigned time);
	virtual void playTone(unsigned freq, unsigned time);
private:
	d2Model::D2RobotModel *mD2Model;
};
}
}
}
}
}
}
