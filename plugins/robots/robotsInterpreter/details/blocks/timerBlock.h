#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "block.h"
#include "../abstractTimer.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class TimerBlock : public Block
{
	Q_OBJECT

public:
	explicit TimerBlock(AbstractTimer * const timer /* Takes ownership */);
	virtual ~TimerBlock() {}

	virtual void run();

private slots:
	void timeout();

private:
	AbstractTimer * const mTimer;
};

}
}
}
}
}
