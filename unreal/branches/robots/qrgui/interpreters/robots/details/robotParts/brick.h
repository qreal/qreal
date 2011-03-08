#pragma once
#pragma once
#include <QtCore/QObject>
#include "../robotImplementations/brickImplementations/abstractBrickImplementation.h"
#include "../../../../kernel/ids.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class Brick : public QObject
{
	Q_OBJECT
public:
	Brick(robotImplementations::brickImplementations::AbstractBrickImplementation *brickImpl);
	~Brick();
	void beep(unsigned time);
	void playTone(unsigned freq, unsigned time);
	robotImplementations::brickImplementations::AbstractBrickImplementation &brickImpl();
private:
	robotImplementations::brickImplementations::AbstractBrickImplementation *mBrickImpl;
};

}
}
}
}
}
