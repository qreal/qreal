#pragma once

#include <qrutils/interpreter/block.h>
#include <kitBase/robotModel/robotModelInterface.h>

namespace utils {
class AbstractTimer;
}

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// Interpreter`s implementation for timer block.
/// Waits for the given amount of time using robot model`s timeline.
/// @todo Why it does not inherit WaitBlock?
class TimerBlock : public qReal::interpretation::Block
{
	Q_OBJECT

public:
	explicit TimerBlock(kitBase::robotModel::RobotModelInterface &robotModel);
	~TimerBlock() override;

	void run() override;

private slots:
	void timeout();

private:
	utils::AbstractTimer * const mTimer;  // Has ownership (via Qt parent-child system).
};

}
}
}
