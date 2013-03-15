#include "sensorsGraph.h"
#include "ui_sensorsGraph.h"

using namespace qReal::interpreters::robots::sensorsGraph;

SensorsGraph::SensorsGraph(const utils::ExpressionsParser *parser, QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::SensorsGraph)
	, mParser(parser)
	, mCurrentSlot(0)
{
	mUi->setupUi(this);
	initGui();
	makeConnections();

	mUpdateTimer.start(mUpdateRate);
	connect(&mUpdateTimer, SIGNAL(timeout()), this, SLOT(updateValues()));
}

SensorsGraph::~SensorsGraph()
{
	delete mPlotFrame;
	delete mUi;
}

void SensorsGraph::addTrackingObject(int const index, QString const inParserName, QString const displayName)
{
	TrackObject newObject(index, inParserName, displayName);
	if (mWatchList.contains(newObject))
		mWatchList.removeOne(newObject);

	mWatchList.append(newObject);
	watchListChanged();
}

void SensorsGraph::removeTracking(const int index)
{
	TrackObject temp(index);
	mWatchList.removeOne(temp);
	watchListChanged();
}

void SensorsGraph::initGui()
{
	setupToolElements();

	mToolLayout.addWidget(&mStopButton);
	mToolLayout.addWidget(&mStartButton);
	mToolLayout.addWidget(&mZoomInButton);
	mToolLayout.addWidget(&mZoomOutButton);
	mToolLayout.addWidget(&mResetButton);

	mPlotFrame = new SensorViewer(this);

	mUi->mainLayout->addWidget(mPlotFrame, 0, 0);
	mUi->mainLayout->addLayout(&mToolLayout, 0, 1);
	mUi->mainLayout->addWidget(&mSlotComboBox, 2, 0);
}

void SensorsGraph::setupToolElements()
{
	QSize iconSize(20, 20);

	mStopButton.setIcon(QPixmap(":/icons/robots_stop.png"));
	mStopButton.setIconSize(iconSize);

	mStartButton.setIcon(QPixmap(":/icons/robots_run.png"));
	mStartButton.setIconSize(iconSize);

	mResetButton.setIcon(QPixmap(":/icons/reset_btn.png"));
	mResetButton.setIconSize(iconSize);

	mZoomInButton.setIcon(QPixmap(":/icons/zoomIn_btn.png"));
	mZoomInButton.setIconSize(iconSize);

	mZoomOutButton.setIcon(QPixmap(":/icons/zoomOut_btn.png"));
	mZoomOutButton.setIconSize(iconSize);
}

void SensorsGraph::makeConnections()
{
	connect(&mStopButton, SIGNAL(clicked()), mPlotFrame, SLOT(stopJob()));
	connect(&mStartButton, SIGNAL(clicked()), mPlotFrame, SLOT(startJob()));
	connect(&mResetButton, SIGNAL(clicked()), mPlotFrame, SLOT(clear()));
	connect(&mZoomInButton, SIGNAL(clicked()), mPlotFrame, SLOT(zoomIn()));
	connect(&mZoomOutButton, SIGNAL(clicked()), mPlotFrame, SLOT(zoomOut()));
	connect(&mSlotComboBox, SIGNAL(currentIndexChanged(int)), mPlotFrame, SLOT(onSensorChange(int)));
	connect(&mSlotComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentSensor(int)));
}

void SensorsGraph::watchListChanged()
{
	mSlotComboBox.clear();
	if (mWatchList.isEmpty())
		return;

	foreach (TrackObject const &item, mWatchList) {
		mSlotComboBox.addItem(tr(item.displayName.toUtf8()), item.index);
	}
}

void SensorsGraph::sensorsInput(int const slotIndex, qreal const value)
{
	if (mCurrentSlot == slotIndex)
		mPlotFrame->setNextValue(value);
}

void SensorsGraph::setCurrentSensor(const int newSlotIndex)
{
	mCurrentSlot = newSlotIndex;
}

void SensorsGraph::startJob()
{
	if (mWatchList.isEmpty())
		return;
	mUpdateTimer.start(mUpdateRate);
	mPlotFrame->startJob();
}

void SensorsGraph::updateValues()
{
	int const notExists = -1;
	QMap<QString, QString> *variables = mParser->getVariablesForWatch();

	TrackObject currentObject(mCurrentSlot);
	int index = mWatchList.indexOf(currentObject);
	if (index != notExists)
		sensorsInput(mCurrentSlot, variables->value(mWatchList.at(index).inParserName).toDouble());

	delete variables;
}

void SensorsGraph::stopJob()
{
	mUpdateTimer.stop();
	mPlotFrame->stopJob();
}
