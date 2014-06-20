#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QAction>

#include <qrgui/mainwindow/projectManager/projectManagementInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrutils/watchListWindow.h>

#include <interpreterBase/robotModel/robotModelManagerInterface.h>
#include <interpreterBase/devicesConfigurationProvider.h>

#include "details/thread.h"
#include "details/blocksTable.h"
#include "details/sensorVariablesUpdater.h"
#include "details/autoconfigurer.h"

#include "interpreterInterface.h"

namespace interpreterCore {
namespace interpreter {

class Interpreter : public InterpreterInterface, public interpreterBase::DevicesConfigurationProvider
{
	Q_OBJECT

public:
	Interpreter(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface &logicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, qReal::ProjectManagementInterface const &projectManager
			, BlocksFactoryManagerInterface &blocksFactoryManager
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, utils::ExpressionsParser &parser
			, QAction &connectToRobotAction
			);

	~Interpreter() override;

public slots:
	void connectToRobot() override;
	void interpret() override;
	void stopRobot() override;
	int timeElapsed() const override;

private slots:
	void threadStopped();
	void newThread(qReal::Id const &startBlockId);

	void connectedSlot(bool success);
	void devicesConfiguredSlot();

	/// Actions when robot disconnect
	void disconnectSlot();

private:
	enum InterpreterState {
		interpreting
		, waitingForDevicesConfiguredToLaunch
		, idle
	};

	void addThread(details::Thread * const thread);

	void reportError(QString const &message);

	qReal::GraphicalModelAssistInterface const &mGraphicalModelApi;
	qReal::LogicalModelAssistInterface &mLogicalModelApi;
	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;

	InterpreterState mState;
	quint64 mInterpretationStartedTimestamp;
	QList<details::Thread *> mThreads;  // Has ownership
	interpreterBase::robotModel::RobotModelManagerInterface const &mRobotModelManager;
	details::BlocksTable *mBlocksTable;  // Has ownership

	/// Action responsible for the connection to the robot
	QAction &mActionConnectToRobot;

	details::SensorVariablesUpdater mSensorVariablesUpdater;
	details::Autoconfigurer mAutoconfigurer;
};

}
}
