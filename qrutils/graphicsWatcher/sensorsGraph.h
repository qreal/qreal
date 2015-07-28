/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	explicit SensorsGraph(const qrtext::DebuggerInterface &parser, QWidget *parent = 0);
	~SensorsGraph();

	/// Hides or shows start and stop buttons.
	void setStartStopButtonsVisible(bool visible);

	/// add new element to watch list
	/// @param index is like slot number
	/// @param inParserName how to find value in expressionsParser
	/// @param displayName will be shown in ComboBox
	void addTrackingObject(const int index, const QString &inParserName, const QString &displayName);
	void removeTracking(const int index);

	/// Removes all tracking objects
	void clearTrackingObjects();

	/// paintEvent makes resize operations
	void paintEvent(QPaintEvent *event = 0);

	static const int readSensorDefaultInterval = 50;
	static const int autoscalingDefault = 3000;
	static const int textUpdateDefault = 500;

public slots:
	void sensorsInput(const int slotIndex, const qreal value);
	void setCurrentSensor(const int newSlotIndex);
	void startJob();
	void stopJob();
	void resetAll();

	/// user settings
	/// @arg fpsDelay is interval in ms for frame update
	/// @arg autoScaleDelay is interval on ms for autoscaling the plot
	/// @arg textInfoUpdateDelay is interval in ms for info
	void configureUpdateIntervals(
			const int &readSensorsInterval
			, const int &autoScaleInterval
			, const int &textUpdaterInterval
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
	const qrtext::DebuggerInterface &mParser;
	struct TrackObject;
	QList<TrackObject> mWatchList;

	/// update sensors value interval in ms
	const int mUpdateInterval;
	int mCurrentSlot;
};

}
}

