#pragma once

#include <QtCore/QTimer>

#include "utils/abstractTimer.h"

#include "utilsDeclSpec.h"

namespace utils {

class ROBOTS_UTILS_EXPORT RealTimer : public AbstractTimer
{
public:
	RealTimer();

	virtual void start(int ms);

private:
	QTimer mTimer;
};

}
