#pragma once

#include <QtCore/QObject>

#include "../../models/graphicalModelAssistApi.h"
#include "../../models/logicalModelAssistApi.h"
#include "../../mainwindow/errorReporter.h"
#include "../../mainwindow/mainWindowInterpretersInterface.h"

#include "robotCommunicationInterface.h"
#include "sensorType.h"
#include "details/robotModel.h"
#include "details/thread.h"
#include "details/blocksTable.h"

namespace qReal {
namespace interpreters {
namespace robots {

class Interpreter : public QObject
{
	Q_OBJECT

public:
	Interpreter(models::GraphicalModelAssistApi const &graphicalModelApi
			, models::LogicalModelAssistApi const &logicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &mainWindowInterface
			, RobotCommunicationInterface * const robotCommunicationInterface);
	~Interpreter();

	void interpret(Id const &currentDiagramId);
	void configureSensors(SensorType::SensorType const &port1, SensorType::SensorType const &port2
			, SensorType::SensorType const &port3, SensorType::SensorType const &port4);
	void stop();

private slots:
	void threadStopped();

private:
	enum InterpreterState {
		interpreting
		, idle
	};

	models::GraphicalModelAssistApi const &mGraphicalModelApi;
	models::LogicalModelAssistApi const &mLogicalModelApi;
	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;

	InterpreterState mState;
	QList<details::Thread *> mThreads;  // Has ownership
	details::RobotModel mRobotModel;
	details::BlocksTable *mBlocksTable;  // Has ownership

	Id const findStartingElement(Id const &diagram) const;
};

}
}
}
