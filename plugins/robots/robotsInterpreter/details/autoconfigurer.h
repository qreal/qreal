#pragma once

#include "blocksTable.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class Autoconfigurer {
public:
	Autoconfigurer(GraphicalModelAssistInterface const &graphicalModelApi
			, BlocksTable * const blocksTable
			, ErrorReporterInterface * const errorReporter
			, RobotModel *robotModel
			);

	bool configure(Id const &diagram);
private:
	GraphicalModelAssistInterface const *mGraphicalModelApi;  // Does not have ownership
	BlocksTable *mBlocksTable;  // Does not have ownership
	ErrorReporterInterface * const mErrorReporter;  // Does not have ownership
	RobotModel *mRobotModel;

	QVector<sensorType::SensorTypeEnum> mUsedSensors;
};

}
}
}
}
