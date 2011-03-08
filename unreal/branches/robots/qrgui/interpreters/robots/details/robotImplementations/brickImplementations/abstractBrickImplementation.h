#pragma once
#include <QtCore/QObject>

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace brickImplementations {

class AbstractBrickImplementation : public QObject
{
	Q_OBJECT
public:
	AbstractBrickImplementation();
	virtual void beep(unsigned time) = 0;
	virtual void playTone(unsigned freq, unsigned time) = 0;
};
}
}
}
}
}
}

