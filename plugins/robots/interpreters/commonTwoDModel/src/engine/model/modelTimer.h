#pragma once

#include <utils/abstractTimer.h>

#include "timeline.h"

namespace twoDModel {
namespace model {

/// Timer implementation for 2D model. Used in TimerBlock and BeepBlock
class ModelTimer : public utils::AbstractTimer
{
	Q_OBJECT

public:
	explicit ModelTimer(Timeline const *timeline /* Doesn`t take ownership */);

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
