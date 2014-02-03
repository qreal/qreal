#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QAction>

#include <qrkernel/ids.h>
#include <qrgui/mainwindow/projectManager/projectManagementInterface.h>

#include <interpreterBase/robotModel/robotModelManagerInterface.h>

#include "details/thread.h"
#include "details/blocksTable.h"
#include "details/sensorVariablesUpdater.h"
#include "details/robotsBlockParser.h"

#include "interpreterInterface.h"

namespace interpreterCore {
namespace interpreter {

class Interpreter : public InterpreterInterface
{
	Q_OBJECT

public:
	Interpreter(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface &logicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, qReal::ProjectManagementInterface const &projectManager
			, interpreterBase::blocksBase::BlocksFactoryInterface * const blocksFactory  // Takes ownership.
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, QAction &connectToRobotAction
			);

	~Interpreter() override;

	virtual interpreterBase::blocksBase::BlockParserInterface &parser() const;

	qReal::IdList providedBlocks() const override;

public slots:
	void connectToRobot() override;
	void interpret() override;
	void stopRobot() override;
//	void showD2ModelWidget(bool isVisible);
//	void showWatchList();
//	void onTabChanged(Id const &diagramId, bool enabled);
//	void saveSensorConfiguration();
//	void updateGraphicWatchSensorsList();

private slots:
	void threadStopped();
	void newThread(qReal::Id const &startBlockId);

	void connectedSlot(bool success);
	void sensorsConfiguredSlot();

	/// Actions when robot disconnect
	void disconnectSlot();

//	void reportError(QString const &message);

//	void on2dModelChanged(QDomDocument const &xml);
//	void loadSensorConfiguration(Id const &diagramId);

private:
//	void setRobotImplementation(details::robotImplementations::AbstractRobotModelImplementation *robotImpl);
	void addThread(details::Thread * const thread);
//	void updateSensorValues(QString const &sensorVariableName, int sensorValue);
//	void resetVariables();

	enum InterpreterState {
		interpreting
		, waitingForSensorsConfiguredToLaunch
		, idle
	};

	qReal::GraphicalModelAssistInterface const *mGraphicalModelApi;  // Does not have ownership
	qReal::LogicalModelAssistInterface *mLogicalModelApi;  // Does not have ownership
	qReal::gui::MainWindowInterpretersInterface *mInterpretersInterface;  // Does not have ownership

	InterpreterState mState;
	QList<details::Thread *> mThreads;  // Has ownership
	interpreterBase::robotModel::RobotModelManagerInterface const &mRobotModelManager;
	details::BlocksTable *mBlocksTable;  // Has ownership
	details::RobotsBlockParser *mParser;  // Has ownership

	bool mConnected;

//	robots::enums::robotModelType::robotModelTypeEnum mImplementationType;

//	utils::WatchListWindow *mWatchListWindow;  // Doesn`t have ownership

//	utils::sensorsGraph::SensorsGraph *mGraphicsWatch;  // Doesn`t have ownership

	/// Action responsible for the connection to the robot
	QAction &mActionConnectToRobot;

	details::SensorVariablesUpdater mSensorVariablesUpdater;

//	QString mLastCommunicationValue;
};

}
}
