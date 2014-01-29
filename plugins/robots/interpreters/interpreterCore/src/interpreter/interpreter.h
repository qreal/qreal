#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QAction>

#include <qrkernel/ids.h>
//#include <qrutils/watchListWindow.h>
//#include <qrutils/graphicsWatcher/sensorsGraph.h>
#include <qrgui/mainwindow/projectManager/projectManagementInterface.h>

#include <interpreterBase/robotModel/robotModelInterface.h>

//#include "details/robotCommunication/robotCommunicator.h"
//#include "sensorConstants.h"
//#include "details/robotParts/robotModel.h"
#include "details/thread.h"
#include "details/blocksTable.h"
//#include "details/d2RobotModel/d2RobotModel.h"


#include "details/robotsBlockParser.h"
//#include "details/robotCommunication/bluetoothRobotCommunicationThread.h"
//#include "details/sensorsConfigurationWidget.h"
//#include "details/nxtDisplay.h"

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
			, interpreterBase::baseBlocks::BlocksFactoryInterface * const blocksFactory  // Takes ownership.
			, interpreterBase::robotModel::RobotModelInterface * const robotModel  // Does not take ownership.
			, QAction &connectToRobotAction
			);

	~Interpreter() override;

	virtual interpreterBase::baseBlocks::BlockParserInterface &parser() const;

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
//	void runTimer();
//	void readSensorValues();
//	void slotFailure();

//	void responseSlot1(int sensorValue);
//	void responseSlot2(int sensorValue);
//	void responseSlot3(int sensorValue);
//	void responseSlot4(int sensorValue);
//	void responseSlotA(int encoderValue);
//	void responseSlotB(int encoderValue);
//	void responseSlotC(int encoderValue);

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
	interpreterBase::robotModel::RobotModelInterface *mRobotModel;  // Does not have ownership
	details::BlocksTable *mBlocksTable;  // Has ownership
	details::RobotsBlockParser *mParser;
//	QTimer mTimer;
//	details::d2Model::D2ModelWidget *mD2ModelWidget;
//	details::d2Model::D2RobotModel *mD2RobotModel;
//	details::RobotCommunicator* const mRobotCommunication;

	bool mConnected;

//	robots::enums::robotModelType::robotModelTypeEnum mImplementationType;

//	utils::WatchListWindow *mWatchListWindow;  // Doesn`t have ownership

//	utils::sensorsGraph::SensorsGraph *mGraphicsWatch;  // Doesn`t have ownership

	/// Action responsible for the connection to the robot
	QAction &mActionConnectToRobot;

//	QString mLastCommunicationValue;
};

}
}
