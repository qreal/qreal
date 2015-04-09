#pragma once

#include <utils/abstractTimer.h>

#include "twoDModel/engine/model/timeline.h"

namespace twoDModel {
namespace model {

/// Timer implementation for 2D model. Used in TimerBlock and BeepBlock
class ModelTimer : public utils::AbstractTimer
{
	Q_OBJECT

public:
	explicit ModelTimer(const Timeline *timeline /* Doesn`t take ownership */);

	void start(int ms) override;
	void stop() override;

private slots:
	void onTick();

private:
	const Timeline *mTimeline;
	int mTimeToWait;
	bool mListening;
	int mTimePast;
};

}
}
