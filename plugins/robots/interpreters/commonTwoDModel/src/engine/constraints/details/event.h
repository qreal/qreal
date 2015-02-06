#pragma once

#include "defines.h"

namespace twoDModel {
namespace constraints {
namespace details {

class Event : public QObject
{
	Q_OBJECT

public:
	Event(const QString &id
		, const Condition &condition
		, const Trigger &trigger
		, bool dropsOnFire = true);

	QString id() const;

	bool isAlive() const;

	void setUp();
	void drop();

	void check();

	void setCondition(const Condition &condition);

signals:
	void settedUp();
	void dropped();
	void fired();

private:
	const QString mId;
	bool mIsAlive;
	Condition mCondition;
	const Trigger mTrigger;
	bool mDropsOnFire;
};

}
}
}
