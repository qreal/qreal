#pragma once

//#include <interpreterBase/robotModel/sensorId.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

#include "blocksTable.h"

namespace interpreterCore {
namespace interpreter {
namespace details {

class Autoconfigurer {
public:
	Autoconfigurer(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, BlocksTable * const blocksTable
			, qReal::ErrorReporterInterface * const errorReporter
			, interpreterBase::robotModel::RobotModelInterface *robotModel
			);

//	QVector<interpreterBase::robotModel::SensorId> configure(qReal::Id const &diagram, bool *success);

private:
	qReal::GraphicalModelAssistInterface const *mGraphicalModelApi;  // Does not have ownership
	BlocksTable *mBlocksTable;  // Does not have ownership
	qReal::ErrorReporterInterface * const mErrorReporter;  // Does not have ownership
	interpreterBase::robotModel::RobotModelInterface *mRobotModel;  // Does not have ownership

//	QVector<interpreterBase::robotModel::SensorId> mUsedSensors;
};

}
}
}
