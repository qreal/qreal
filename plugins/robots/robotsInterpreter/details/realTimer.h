#pragma once

#include <QtCore/QTimer>

#include "abstractTimer.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

/// Timer implementation for real-life time
class RealTimer : public AbstractTimer
{
public:
	RealTimer();

	virtual void start(int ms);

private:
	QTimer mTimer;
};

}
}
}
}
