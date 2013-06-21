#pragma once

#include <QtCore/QObject>

#include "../robotImplementations/brickImplementations/abstractBrickImplementation.h"
#include "../../../../../qrkernel/ids.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class Brick : public QObject
{
	Q_OBJECT

public:
	explicit Brick(robotImplementations::brickImplementations::AbstractBrickImplementation *brickImpl);
	~Brick();
	void beep(unsigned time);
	void playTone(unsigned freq, unsigned time);
	robotImplementations::brickImplementations::AbstractBrickImplementation &brickImpl();
	void setImplementation(robotImplementations::brickImplementations::AbstractBrickImplementation *brickImpl);

private:
	robotImplementations::brickImplementations::AbstractBrickImplementation *mBrickImpl;  // Doesn't have ownership
};

}
}
}
}
}
