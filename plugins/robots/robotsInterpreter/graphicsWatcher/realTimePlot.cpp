#include "realTimePlot.h"
#include "ui_realTimePlot.h"

RealTimePlot::RealTimePlot(const utils::ExpressionsParser *parser, QWidget *parent) :
	QWidget(parent)
	, mUi(new Ui::RealTimePlot)
	, mCurrentSlot(0)
	, mParser(parser)
	, mUpdateRate(50)
{
	mUi->setupUi(this);
	initGui();
	makeConnections();

	mUpdateTimer.start(mUpdateRate);
	connect(&mUpdateTimer, SIGNAL(timeout()), this, SLOT(updateValues()));
}


RealTimePlot::~RealTimePlot()
{
	delete mPlotFrame;
	delete mUi;
}

void RealTimePlot::initGui()
{
	setupToolElements();

	mToolLayout.addWidget(&mStopButton);
	mToolLayout.addWidget(&mStartButton);
	mToolLayout.addWidget(&mZoomInButton);
	mToolLayout.addWidget(&mZoomOutButton);
	mToolLayout.addWidget(&mResetButton);

	mPlotFrame = new SensorGraph(this);

	mUi->mainLayout->addWidget(mPlotFrame, 0, 0);
	mUi->mainLayout->addLayout(&mToolLayout, 0, 1);
	mUi->mainLayout->addWidget(&mSlotComboBox, 2, 0);
}

void RealTimePlot::setupToolElements()
{
	QSize iconSize(20, 20);

	mStopButton.setIcon(QPixmap("../plugins/robots/robotsInterpreter/graphicsWatcher/stop_btn.png"));
	mStopButton.setIconSize(iconSize);

	mStartButton.setIcon(QPixmap("../plugins/robots/robotsInterpreter/graphicsWatcher/start_btn.png"));
	mStartButton.setIconSize(iconSize);

	mResetButton.setIcon(QPixmap("../plugins/robots/robotsInterpreter/graphicsWatcher/reset_btn.png"));
	mResetButton.setIconSize(iconSize);

	mZoomInButton.setIcon(QPixmap("../plugins/robots/robotsInterpreter/graphicsWatcher/zoomIn_btn.png"));
	mZoomInButton.setIconSize(iconSize);

	mZoomOutButton.setIcon(QPixmap("../plugins/robots/robotsInterpreter/graphicsWatcher/zoomOut_btn.png"));
	mZoomOutButton.setIconSize(iconSize);

	mSlotComboBox.addItem("Sensor 1", 0);
	mSlotComboBox.addItem("Sensor 2", 1);
	mSlotComboBox.addItem("Sensor 3", 2);
	mSlotComboBox.addItem("Sensor 4", 3);

}

void RealTimePlot::makeConnections()
{
	connect(&mStopButton, SIGNAL(clicked()), mPlotFrame, SLOT(stopJob()));
	connect(&mStartButton, SIGNAL(clicked()), mPlotFrame, SLOT(startJob()));
	connect(&mResetButton, SIGNAL(clicked()), mPlotFrame, SLOT(clear()));
	connect(&mZoomInButton, SIGNAL(clicked()), mPlotFrame, SLOT(zoomIn()));
	connect(&mZoomOutButton, SIGNAL(clicked()), mPlotFrame, SLOT(zoomOut()));
	connect(&mSlotComboBox, SIGNAL(currentIndexChanged(int)), mPlotFrame, SLOT(onSensorChange(int)));
	connect(&mSlotComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setMainSensor(int)));
}

void RealTimePlot::sensorOne(const qreal value)
{
	if (mCurrentSlot == 0)
		mPlotFrame->setNextValue(value);
}

void RealTimePlot::sensorTwo(const qreal value)
{
	if (mCurrentSlot == 1)
		mPlotFrame->setNextValue(value);
}

void RealTimePlot::sensorThree(const qreal value)
{
	if (mCurrentSlot == 2)
		mPlotFrame->setNextValue(value);
}

void RealTimePlot::sensorFour(const qreal value)
{
	if (mCurrentSlot == 3)
		mPlotFrame->setNextValue(value);
}

void RealTimePlot::setMainSensor(const int newSlotIndex)
{
	mCurrentSlot = newSlotIndex;
}

void RealTimePlot::startJob()
{
	mUpdateTimer.start(mUpdateRate);
	mPlotFrame->startJob();
}

void RealTimePlot::updateValues()
{
	QMap<QString, QString> *variables = mParser->getVariablesForWatch();

	sensorOne(variables->value("Sensor1").toDouble());
	sensorTwo(variables->value("Sensor2").toDouble());
	sensorThree(variables->value("Sensor3").toDouble());
	sensorFour(variables->value("Sensor4").toDouble());

	delete variables;
}

void RealTimePlot::stopJob()
{
	mUpdateTimer.stop();
	mPlotFrame->stopJob();
}
