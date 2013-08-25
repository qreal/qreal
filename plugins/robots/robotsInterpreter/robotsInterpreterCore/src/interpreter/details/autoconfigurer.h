#pragma once

#include <robotsInterpreterCore/robotModel/sensorId.h>
#include <robotsInterpreterCore/robotModel/robotModelInterface.h>

#include "blocksTable.h"

namespace robotsInterpreterCore {
namespace interpreter {
namespace details {

class Autoconfigurer {
public:
	Autoconfigurer(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, BlocksTable * const blocksTable
			, qReal::ErrorReporterInterface * const errorReporter
			, robotModel::RobotModelInterface *robotModel
			);

	QVector<robotModel::SensorId> configure(qReal::Id const &diagram, bool *success);

private:
	qReal::GraphicalModelAssistInterface const *mGraphicalModelApi;  // Does not have ownership
	BlocksTable *mBlocksTable;  // Does not have ownership
	qReal::ErrorReporterInterface * const mErrorReporter;  // Does not have ownership
	robotModel::RobotModelInterface *mRobotModel;  // Does not have ownership

	QVector<robotModel::SensorId> mUsedSensors;
};

}
}
}
