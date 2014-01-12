#pragma once

#include <utils/abstractTimer.h>
#include "timeline.h"

namespace twoDModel {

/// Timer implementation for 2D model. Used in TimerBlock and BeepBlock
class D2ModelTimer : public utils::AbstractTimer
{
	Q_OBJECT

public:
	explicit D2ModelTimer(Timeline const *timeline /* Doesn`t take ownership */);

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
