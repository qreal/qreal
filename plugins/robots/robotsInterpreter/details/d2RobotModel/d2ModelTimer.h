#pragma once

#include "../abstractTimer.h"
#include "timeline.h"

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace details
{
namespace d2Model
{

/// Timer implementation for 2D model. Used in TimerBlock and BeepBlock
class D2ModelTimer : public AbstractTimer
{
	Q_OBJECT

public:
	D2ModelTimer(Timeline const *timeline /* Doesn`t take ownership */);

	virtual void start(int ms);

private slots:
	void onTick();

private:
	Timeline const *mTimeline;
	int mTimeToWait;
	bool mListening;
	int mTimePast;
};

}
}
}
}
}