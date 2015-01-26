#pragma once

#include <qrutils/interpreter/block.h>

#include <interpreterBase/blocksBase/blocksFactoryInterface.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

namespace interpreterBase {
namespace blocksBase {

/// Base class for all blocks implementations used in robots interpreter.
class ROBOTS_INTERPRETER_BASE_EXPORT RobotsBlock : public qReal::interpretation::Block
{
	Q_OBJECT

public:
	virtual QMap<robotModel::PortInfo, robotModel::DeviceInfo> usedDevices();

	/// Initializes this block with external components that may be used during the intepretetation process.
	/// @todo Not good.
	void init(qReal::Id const &graphicalId
			, qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, qReal::ErrorReporterInterface * const errorReporter
			, robotModel::RobotModelManagerInterface const &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguageToolbox
			);

protected:
	RobotsBlock();

	/// Reference to a robot model which is used by this block.
	robotModel::RobotModelInterface &model();

	robotModel::RobotModelManagerInterface const *mRobotModelManager;  // Doesn't have ownership.

private:
	// Just to make it private
	void init(qReal::Id const &graphicalId
			, qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, qReal::ErrorReporterInterface * const errorReporter
			, qrtext::LanguageToolboxInterface &textLanguageToolbox
			);
};

}
}
