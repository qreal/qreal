#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QComboBox>
#include <QtCore/QTimer>

#include <qrtext/debuggerInterface.h>

#include "sensorViewer.h"

#include "qrutils/utilsDeclSpec.h"

namespace Ui {
class SensorsGraph;
}

namespace utils {
namespace sensorsGraph {

/// @class SensorsGraph is widget to show sensors state
class QRUTILS_EXPORT SensorsGraph : public QWidget
{
	Q_OBJECT

public:
	explicit SensorsGraph(qrtext::DebuggerInterface const &parser, QWidget *parent = 0);
	~SensorsGraph();

	/// add new element to watch list
	/// @param index is like slot number
	/// @param inParserName how to find value in expressionsParser
	/// @param displayName will be shown in ComboBox
	void addTrackingObject(int const index, QString const &inParserName, QString const &displayName);
	void removeTracking(int const index);

	/// Removes all tracking objects
	void clearTrackingObjects();

	/// paintEvent makes resize operations
	void paintEvent(QPaintEvent *event = 0);

	static int const readSensorDefaultInterval = 50;
	static int const autoscalingDefault = 3000;
	static int const textUpdateDefault = 500;

public slots:
	void sensorsInput(int const slotIndex, qreal const value);
	void setCurrentSensor(int const newSlotIndex);
	void startJob();
	void stopJob();
	void resetAll();

	/// user settings
	/// @arg fpsDelay is interval in ms for frame update
	/// @arg autoScaleDelay is interval on ms for autoscaling the plot
	/// @arg textInfoUpdateDelay is interval in ms for info
	void configureUpdateIntervals(
			int const &readSensorsInterval
			, int const &autoScaleInterval
			, int const &textUpdaterInterval
	);

protected:
	void initGui();
	void setupToolElements();
	void makeConnections();
	void watchListChanged();

private slots:
	void updateValues();

private:
	Ui::SensorsGraph *mUi;
	SensorViewer *mPlotFrame;
	QVBoxLayout mToolLayout;
	QToolButton mStopButton;
	QToolButton mStartButton;
	QToolButton mZoomInButton;
	QToolButton mZoomOutButton;
	QToolButton mResetButton;
	QToolButton mSaveButton;
	QComboBox mSlotComboBox;
	QTimer mUpdateTimer;
	qrtext::DebuggerInterface const &mParser;
	struct TrackObject;
	QList<TrackObject> mWatchList;

	/// update sensors value interval in ms
	int const mUpdateInterval;
	int mCurrentSlot;
};

}
}

