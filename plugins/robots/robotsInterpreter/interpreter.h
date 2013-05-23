#pragma once

#include <QtCore/QObject>

#include "../../../qrkernel/ids.h"

#include "details/robotCommunication/robotCommunicator.h"
#include "sensorConstants.h"
#include "details/robotParts/robotModel.h"
#include "details/thread.h"
#include "details/blocksTable.h"
#include "details/d2RobotModel/d2RobotModel.h"

#include "../../../qrutils/watchListWindow.h"
#include "../../../qrgui/mainwindow/projectManager/projectManagementInterface.h"


#include "details/robotsBlockParser.h"
#include "details/robotCommunication/bluetoothRobotCommunicationThread.h"
#include "details/sensorsConfigurationWidget.h"

namespace qReal {
namespace interpreters {
namespace robots {

class Interpreter : public QObject
{
	Q_OBJECT

public:
	Interpreter();
	virtual ~Interpreter();

	virtual void init(GraphicalModelAssistInterface &graphicalModelApi
			, LogicalModelAssistInterface const &logicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, qReal::ProjectManagementInterface const &projectManager
	);

	details::RobotModel *robotModel();
	void setRobotModel(details::RobotModel * const robotModel);
	void configureSensors(sensorType::SensorTypeEnum const &port1
			, sensorType::SensorTypeEnum const &port2
			, sensorType::SensorTypeEnum const &port3
			, sensorType::SensorTypeEnum const &port4);
	void setRobotImplementation(robotModelType::robotModelTypeEnum implementationType);

	void setD2ModelWidgetActions(QAction *runAction, QAction *stopAction);
	void closeD2ModelWidget();
	void closeWatchList();
	void setRobotModelType(robotModelType::robotModelTypeEnum robotModelType);
	void setCommunicator(const QString &valueOfCommunication, const QString &portName);

	/// Assigning a value to the field mActionConnectToRobot
	void setConnectRobotAction(QAction *actionConnect);

	void setNoiseSettings();

	/// Enable Run and Stop buttons on 2d model widget
	void enableD2ModelWidgetRunStopButtons();

	/// Disable Run and Stop buttons on 2d model widget, when running current diagram is impossible
	void disableD2ModelWidgetRunStopButtons();

	utils::WatchListWindow *watchWindow() const;
	void connectSensorConfigurer(details::SensorsConfigurationWidget *configurer) const;

signals:
	void noiseSettingsChanged();
	void noiseSettingsChangedBy2DModelWidget();

	void sensorsConfigurationChanged();

public slots:
	void connectToRobot();
	void interpret();
	void stopRobot();
	void showD2ModelWidget(bool isVisible);
	void showWatchList();
	void onTabChanged(Id const &diagramId, bool enabled);
	void saveSensorConfiguration();

private slots:
	void threadStopped();
	void newThread(details::blocks::Block * const startBlock);
	void runTimer();
	void readSensorValues();
	void slotFailure();

	void responseSlot1(int sensorValue);
	void responseSlot2(int sensorValue);
	void responseSlot3(int sensorValue);
	void responseSlot4(int sensorValue);
	void responseSlotA(int encoderValue);
	void responseSlotB(int encoderValue);
	void responseSlotC(int encoderValue);

	void connectedSlot(bool success);
	void sensorsConfiguredSlot();

	/// actions when robot disconnect
	void disconnectSlot();

	void reportError(QString const &message);

	void on2dModelChanged(QDomDocument const &xml);
	void loadSensorConfiguration(Id const &diagramId);

private:
	void setRobotImplementation(details::robotImplementations::AbstractRobotModelImplementation *robotImpl);
	Id const findStartingElement(Id const &diagram) const;
	void addThread(details::Thread * const thread);
	void updateSensorValues(QString const &sensorVariableName, int sensorValue);
	void resetVariables();

	enum InterpreterState {
		interpreting
		, waitingForSensorsConfiguredToLaunch
		, idle
	};

	GraphicalModelAssistInterface *mGraphicalModelApi;
	LogicalModelAssistInterface const *mLogicalModelApi;
	qReal::gui::MainWindowInterpretersInterface *mInterpretersInterface;

	InterpreterState mState;
	QList<details::Thread *> mThreads;  // Has ownership
	details::RobotModel *mRobotModel;
	details::BlocksTable *mBlocksTable;  // Has ownership
	RobotsBlockParser *mParser;
	QTimer *mTimer;
	details::d2Model::D2ModelWidget *mD2ModelWidget;
	details::d2Model::D2RobotModel *mD2RobotModel;
	RobotCommunicator* const mRobotCommunication;

	bool mConnected;

	robotModelType::robotModelTypeEnum mImplementationType;

	utils::WatchListWindow *mWatchListWindow;

	/// Action responsible for the connection to the robot
	QAction *mActionConnectToRobot;

	QString mLastCommunicationValue;
};

}
}
}
