#pragma once

#include <QtCore/QObject>
#include <QtCore/QVector>

#include <interpreterBase/blocksBase/block.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// @todo Move abstract blocks to interpreterBase::blocksBase
class EngineCommandBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit EngineCommandBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	virtual void run() = 0;

protected slots:
	void timeout();

protected:
	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
