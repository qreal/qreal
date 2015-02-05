#pragma once

#include "defines.h"

namespace twoDModel {
namespace constraints {
namespace details {

class Event : public QObject
{
	Q_OBJECT

public:
	Event(const Condition &condition, const Trigger &trigger, bool dropsOnFire = true);

	bool isAlive() const;

	void setUp();
	void drop();

	void check();

signals:
	void settedUp();
	void dropped();
	void fired();

private:
	bool mIsAlive;
	const Condition &mCondition;
	const Trigger &mTrigger;
	bool mDropsOnFire;
};

}
}
}
