#pragma once

#include <qrutils/interpreter/block.h>

#include <kitBase/blocksBase/blocksFactoryInterface.h>
#include <kitBase/robotModel/robotModelManagerInterface.h>

namespace kitBase {
namespace blocksBase {

/// Base class for all blocks implementations used in robots interpreter.
class ROBOTS_KIT_BASE_EXPORT RobotsBlock : public qReal::interpretation::Block
{
	Q_OBJECT

public:
	virtual QMap<robotModel::PortInfo, robotModel::DeviceInfo> usedDevices();

	/// Initializes this block with external components that may be used during the intepretetation process.
	/// @todo Not good.
	void init(const qReal::Id &graphicalId
			, const qReal::GraphicalModelAssistInterface &graphicalModelApi
			, const qReal::LogicalModelAssistInterface &logicalModelApi
			, qReal::ErrorReporterInterface * const errorReporter
			, const robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguageToolbox
			);

protected:
	RobotsBlock();

	/// Reference to a robot model which is used by this block.
	robotModel::RobotModelInterface &model();

	const robotModel::RobotModelManagerInterface *mRobotModelManager;  // Doesn't have ownership.

private:
	// Just to make it private
	void init(const qReal::Id &graphicalId
			, const qReal::GraphicalModelAssistInterface &graphicalModelApi
			, const qReal::LogicalModelAssistInterface &logicalModelApi
			, qReal::ErrorReporterInterface * const errorReporter
			, qrtext::LanguageToolboxInterface &textLanguageToolbox
			);
};

}
}
