#pragma once

#include <QtCore/QTimer>

#include "abstractTimer.h"

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace details
{

class NullTimer : public AbstractTimer
{
public:
	virtual void start(int ms);
};

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
