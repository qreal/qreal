#include "sensorsGraph.h"
#include "ui_sensorsGraph.h"

using namespace utils::sensorsGraph;

struct SensorsGraph::TrackObject {
	int index;
	QString inParserName;
	QString displayName;

	TrackObject(int const &id, QString const &internalName, QString const &externalName)
		: index(id)
		, inParserName(internalName)
		, displayName(externalName)
	{
	}

	TrackObject(int const &id)
		: index(id)
		, inParserName("")
		, displayName("")
	{
	}

	bool operator ==(TrackObject const &other) const
	{
		return index == other.index;
	}
};

SensorsGraph::SensorsGraph(utils::ExpressionsParser const *parser, QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::SensorsGraph)
	, mParser(parser)
	, mUpdateInterval(100)
	, mCurrentSlot(0)
{
	mUi->setupUi(this);
	initGui();
	makeConnections();

	mUpdateTimer.start(mUpdateInterval);
	connect(&mUpdateTimer, SIGNAL(timeout()), this, SLOT(updateValues()));
	mPlotFrame->centerOn(mPlotFrame->sceneRect().center());
}

SensorsGraph::~SensorsGraph()
{
	delete mPlotFrame;
	delete mUi;
}

void SensorsGraph::addTrackingObject(int const index, QString const &inParserName, QString const &displayName)
{
	TrackObject newObject(index, inParserName, displayName);
	if (mWatchList.contains(newObject)) {
		mWatchList.removeOne(newObject);
	}

	mWatchList.append(newObject);
	watchListChanged();
}

void SensorsGraph::removeTracking(const int index)
{
	TrackObject temp(index);
	mWatchList.removeOne(temp);
	watchListChanged();
}

void SensorsGraph::clearTrackingObjects()
{
	for (int i = 0; i < mWatchList.count(); ++i) {
		removeTracking(i);
	}
}

void SensorsGraph::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	qreal const verticalCoefficent = 0.8;
	qreal const horizontalCoefficent = 0.8;
	qreal const plotWidth = size().width() * horizontalCoefficent;
	qreal const plotHeight = size().height() * verticalCoefficent;

	mPlotFrame->setSceneRect(-plotWidth, -plotHeight, plotWidth, plotHeight);
	mPlotFrame->setFixedSize(plotWidth, plotHeight);
}

void SensorsGraph::initGui()
{
	setupToolElements();

	mToolLayout.addWidget(&mStopButton, 0);
	mToolLayout.addWidget(&mStartButton, 0);
	mToolLayout.addWidget(&mZoomInButton, 0);
	mToolLayout.addWidget(&mZoomOutButton, 0);
	mToolLayout.addWidget(&mResetButton, 0);
	mToolLayout.addWidget(&mSaveButton, 0);

	mPlotFrame = new SensorViewer(this);

	mUi->mainLayout->addWidget(mPlotFrame, 0, 0);
	mUi->mainLayout->addLayout(&mToolLayout, 0, 1);
	mUi->mainLayout->addWidget(&mSlotComboBox, 1, 0);
	mUi->mainLayout->setAlignment(mPlotFrame, Qt::AlignTop);
	mUi->mainLayout->setAlignment(&mSlotComboBox, Qt::AlignTop);
	mPlotFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void SensorsGraph::setupToolElements()
{
	QSize const iconSize(20, 20);

	mStopButton.setIcon(QPixmap("://graphicsWatcher/icons/stop_btn.png"));
	mStopButton.setIconSize(iconSize);

	mStartButton.setIcon(QPixmap("://graphicsWatcher/icons/start_btn.png"));
	mStartButton.setIconSize(iconSize);

	mResetButton.setIcon(QPixmap("://graphicsWatcher/icons/reset_btn.png"));
	mResetButton.setIconSize(iconSize);

	mZoomInButton.setIcon(QPixmap("://graphicsWatcher/icons/zoomIn_btn.png"));
	mZoomInButton.setIconSize(iconSize);

	mZoomOutButton.setIcon(QPixmap("://graphicsWatcher/icons/zoomOut_btn.png"));
	mZoomOutButton.setIconSize(iconSize);

	mSaveButton.setIcon(QPixmap("://icons/save_as.png"));
	mSaveButton.setIconSize(iconSize);

}

void SensorsGraph::makeConnections()
{
	connect(&mStopButton, SIGNAL(clicked()), mPlotFrame, SLOT(stopJob()));
	connect(&mStartButton, SIGNAL(clicked()), mPlotFrame, SLOT(startJob()));
	connect(&mSaveButton, SIGNAL(clicked()), mPlotFrame, SLOT(saveGraph()));
	connect(&mResetButton, SIGNAL(clicked()), mPlotFrame, SLOT(clear()));
	connect(&mZoomInButton, SIGNAL(clicked()), mPlotFrame, SLOT(zoomIn()));
	connect(&mZoomOutButton, SIGNAL(clicked()), mPlotFrame, SLOT(zoomOut()));
	connect(&mSlotComboBox, SIGNAL(currentIndexChanged(int)), mPlotFrame, SLOT(onSensorChange()));
	connect(&mSlotComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentSensor(int)));
}

void SensorsGraph::watchListChanged()
{
	mSlotComboBox.clear();
	if (mWatchList.isEmpty()) {
		return;
	}

	foreach (TrackObject const &item, mWatchList) {
		mSlotComboBox.addItem(tr(item.displayName.toUtf8()), item.index);
	}
}

void SensorsGraph::sensorsInput(int const slotIndex, qreal const value)
{
	if (mCurrentSlot == slotIndex) {
		mPlotFrame->setNextValue(value);
	}
}

void SensorsGraph::setCurrentSensor(const int newSlotIndex)
{
	mCurrentSlot = newSlotIndex;
}

void SensorsGraph::startJob()
{
	if (mWatchList.isEmpty()) {
		return;
	}
	mUpdateTimer.start(mUpdateInterval);
	mPlotFrame->startJob();
}

void SensorsGraph::saveGraph()
{
	mPlotFrame->saveGraph();
}

void SensorsGraph::updateValues()
{
	int const notExists = -1;
	QMap<QString, Number *> const &variables = mParser->variables();

	TrackObject currentObject(mCurrentSlot);
	int const index = mWatchList.indexOf(currentObject);
	if (index != notExists) {
		Number *const number = variables[mWatchList.at(index).inParserName];
		if (number) {
			sensorsInput(mCurrentSlot, number->value().toDouble());
		}
	}
}

void SensorsGraph::stopJob()
{
	mUpdateTimer.stop();
	mPlotFrame->stopJob();
}

void SensorsGraph::resetAll()
{
	mPlotFrame->zoomIn();
	mPlotFrame->zoomOut();
}

void SensorsGraph::configureUpdateIntervals(int const &readSensorsInterval, int const &autoScaleInterval
		, int const &textUpdaterInterval)
{
	mPlotFrame->configureUserOptions(readSensorsInterval, autoScaleInterval, textUpdaterInterval);
}
