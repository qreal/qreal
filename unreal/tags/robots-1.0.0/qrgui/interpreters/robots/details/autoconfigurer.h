#pragma once

#include "blocksTable.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class Autoconfigurer {
public:
	Autoconfigurer(models::GraphicalModelAssistApi const &graphicalModelApi
			, BlocksTable * const blocksTable
			, gui::ErrorReporter * const errorReporter
			, RobotModel *robotModel
			);

	bool configure(Id const &diagram);
private:
	models::GraphicalModelAssistApi const *mGraphicalModelApi;  // Does not have ownership
	BlocksTable *mBlocksTable;  // Does not have ownership
	gui::ErrorReporter * const mErrorReporter;  // Does not have ownership
	RobotModel *mRobotModel;

	QVector<sensorType::SensorTypeEnum> mUsedSensors;
};

}
}
}
}
