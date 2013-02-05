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

class D2ModelTimer : public AbstractTimer
{
	Q_OBJECT

public:
	D2ModelTimer(Timeline *timeline);

	virtual void start(int ms);

private slots:
	void onTick();

private:
	Timeline *mTimeline;
	int mTimeToWait;
	bool mListening;
	int mTimePast;
};

}
}
}
}
}
