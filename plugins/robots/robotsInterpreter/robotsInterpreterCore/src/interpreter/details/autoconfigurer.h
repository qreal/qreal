#pragma once

#include <robotsInterpreterCore/robotModel/sensorId.h>

#include "blocksTable.h"

namespace robotsInterpreterCore {
namespace interpreter {
namespace details {

class Autoconfigurer {
public:
	Autoconfigurer(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, BlocksTable * const blocksTable
			, qReal::ErrorReporterInterface * const errorReporter
			, RobotModelInterface *robotModel
			);

	bool configure(qReal::Id const &diagram);

private:
	qReal::GraphicalModelAssistInterface const *mGraphicalModelApi;  // Does not have ownership
	BlocksTable *mBlocksTable;  // Does not have ownership
	qReal::ErrorReporterInterface * const mErrorReporter;  // Does not have ownership
	RobotModelInterface *mRobotModel;  // Does not have ownership

	QVector<SensorId> mUsedSensors;
};

}
}
}
