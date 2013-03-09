#include "sensorsGraph.h"
#include "ui_sensorsGraph.h"

using namespace qReal::interpreters::robots::sensorsGraph;

SensorsGraph::SensorsGraph(const utils::ExpressionsParser *parser, QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::SensorsGraph)
	, mParser(parser)
	, mUpdateRate(50)
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

	mStopButton.setIcon(QPixmap(":/icons/stop_btn.png"));
	mStopButton.setIconSize(iconSize);

	mStartButton.setIcon(QPixmap(":/icons/start_btn.png"));
	mStartButton.setIconSize(iconSize);

	mResetButton.setIcon(QPixmap(":/icons/reset_btn.png"));
	mResetButton.setIconSize(iconSize);

	mZoomInButton.setIcon(QPixmap(":/icons/zoomIn_btn.png"));
	mZoomInButton.setIconSize(iconSize);

	mZoomOutButton.setIcon(QPixmap(":/icons/zoomOut_btn.png"));
	mZoomOutButton.setIconSize(iconSize);

	mSlotComboBox.addItem(tr("Sensor 1"), 0);
	mSlotComboBox.addItem(tr("Sensor 2"), 1);
	mSlotComboBox.addItem(tr("Sensor 3"), 2);
	mSlotComboBox.addItem(tr("Sensor 4"), 3);
}

void SensorsGraph::makeConnections()
{
	connect(&mStopButton, SIGNAL(clicked()), mPlotFrame, SLOT(stopJob()));
	connect(&mStartButton, SIGNAL(clicked()), mPlotFrame, SLOT(startJob()));
	connect(&mResetButton, SIGNAL(clicked()), mPlotFrame, SLOT(clear()));
	connect(&mZoomInButton, SIGNAL(clicked()), mPlotFrame, SLOT(zoomIn()));
	connect(&mZoomOutButton, SIGNAL(clicked()), mPlotFrame, SLOT(zoomOut()));
	connect(&mSlotComboBox, SIGNAL(currentIndexChanged(int)), mPlotFrame, SLOT(onSensorChange(int)));
	connect(&mSlotComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setMainSensor(int)));
}

void SensorsGraph::sensorsInput(int const port, qreal const value)
{
	if (mCurrentSlot == port)
		mPlotFrame->setNextValue(value);
}

void SensorsGraph::setMainSensor(const int newSlotIndex)
{
	mCurrentSlot = newSlotIndex;
}

void SensorsGraph::startJob()
{
	mUpdateTimer.start(mUpdateRate);
	mPlotFrame->startJob();
}

void SensorsGraph::updateValues()
{
	QMap<QString, QString> *variables = mParser->getVariablesForWatch();

	int const sensorsCount = 4;
	for (int i = 0; i < sensorsCount; i++) {
		sensorsInput(i, variables->value(QString("Sensor") + QString::number(i + 1)).toDouble());
	}

	delete variables;
}

void SensorsGraph::stopJob()
{
	mUpdateTimer.stop();
	mPlotFrame->stopJob();
}
