#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include <interpreterBase/blocksBase/block.h>
//#include "../abstractTimer.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class TimerBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit TimerBlock(/*AbstractTimer * const timer*/ /* Takes ownership */);
	~TimerBlock() override;

	virtual void run();

private slots:
	void timeout();

private:
//	AbstractTimer * const mTimer;
	/// @todo Reimplement model time timer.
	QTimer mTimer;
};

}
}
}
