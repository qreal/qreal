#pragma once

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QComboBox>
#include <QtCore/QTimer>

#include "../../../qrutils/expressionsParser/expressionsParser.h"
#include "sensorViewer.h"

namespace Ui {
class SensorsGraph;
}

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace sensorsGraph
{
struct TrackObject {
	int index;
	QString inParserName;
	QString displayName;
	TrackObject(int id, QString internalName, QString externalName)
	{
		index = id;
		inParserName = internalName;
		displayName = externalName;
	}
	TrackObject(int id)
	{
		index = id;
		inParserName = QString("");
		displayName = QString("");
	}

	bool operator ==(TrackObject const &other)
	{
		return index == other.index;
	}
};

}
}
}
}

//! @class SensorsGraph is widget to show sensors state
class SensorsGraph : public QWidget
{
	Q_OBJECT

public:
	SensorsGraph(utils::ExpressionsParser const *parser, QWidget *parent = 0);
	~SensorsGraph();

	//! add new element to watch list.
	//! @param index is like slot number
	//! @param inParserName how to find value in expressionsParser
	//! @param displayName will be shown in ComboBox
	void addTrackingObject(int const index, QString const inParserName, QString const displayName);
	void removeTracking(int const index);


public slots:
	void sensorsInput(int const slotIndex, qreal const value);
	void setCurrentSensor(int const newSlotIndex);
	void startJob();
	void stopJob();

protected:
	void initGui();
	void setupToolElements();
	void makeConnections();
	void watchListChanged();

private slots:
	void updateValues();

private:
	Ui::SensorsGraph *mUi;
	qReal::interpreters::robots::sensorsGraph::SensorViewer *mPlotFrame;
	QVBoxLayout mToolLayout;
	QToolButton mStopButton;
	QToolButton mStartButton;
	QToolButton mZoomInButton;
	QToolButton mZoomOutButton;
	QToolButton mResetButton;
	QComboBox mSlotComboBox;
	QTimer mUpdateTimer;
	utils::ExpressionsParser const *mParser;
	QList<qReal::interpreters::robots::sensorsGraph::TrackObject> mWatchList;

	//! update sensors value interval in ms
	static int const mUpdateRate = 50;
	int mCurrentSlot;
};
