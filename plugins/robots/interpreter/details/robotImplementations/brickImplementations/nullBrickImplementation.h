#pragma once
#include "abstractBrickImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace brickImplementations {

class NullBrickImplementation : public AbstractBrickImplementation
{
	Q_OBJECT
public:
	NullBrickImplementation();
	virtual void beep(unsigned time);
	virtual void playTone(unsigned freq, unsigned time);
};

}
}
}
}
}
}
