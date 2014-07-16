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

	void start(int ms) override;
	void stop() override;


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
