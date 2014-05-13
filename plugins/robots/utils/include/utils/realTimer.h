#pragma once

#include <QtCore/QTimer>

#include "abstractTimer.h"
#include "utilsDeclSpec.h"

namespace utils {

/// Timer implementation for real-life time
class ROBOTS_UTILS_EXPORT RealTimer : public AbstractTimer
{
public:
	RealTimer();

	void start(int ms) override;

private:
	QTimer mTimer;
};

}
