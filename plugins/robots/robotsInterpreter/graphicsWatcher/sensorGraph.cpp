#include "sensorGraph.h"

SensorGraph::SensorGraph(QWidget *parent) :
	QGraphicsView(parent)
	, mPenBrush(QBrush(Qt::yellow))
	, fpsDelay(50)
	, autoScaleInterval(4000)
	, updateOutputInterval(1000)
	, stepSize(2)
	, zoomRate(2)
	, mIsZoomed(false)
	, mScaleCoefficient(0)
	, mAutoScaleTimer(0)
	, mUpdateCurrValueTimer(0)
	, mOutputValue(0)
{
	initGraphicsOutput();

	connect(&visualTimer, SIGNAL(timeout()), this, SLOT(visualTimerEvent()));
}

SensorGraph::~SensorGraph()
{
	scene->removeItem(mainPoint);
	delete pointsDataProcessor;
	delete mainPoint;
	delete scene;
}

void SensorGraph::initGraphicsOutput()
{
	scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	scene->setSceneRect(-200, -160, 205, 160);

	setScene(scene);
	setRenderHint(QPainter::Antialiasing, false);

	/// This makes information on left side actual
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setCacheMode(CacheNone);
	setRenderHint(QPainter::Antialiasing);
	setMinimumSize(205, 160);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


	mainPoint = new KeyPoint;
	scene->addItem(mainPoint);

	pointsDataProcessor = new PointsQueueProcessor(scene->sceneRect().height() - 20, scene->sceneRect().left());
}


void SensorGraph::startJob()
{
	visualTimer.start(fpsDelay);
}

void SensorGraph::stopJob()
{
	visualTimer.stop();
}

void SensorGraph::clear()
{

	pointsDataProcessor->clearData();

	foreach (QGraphicsItem *item, scene->items()) {
		QGraphicsLineItem *curLine = qgraphicsitem_cast<QGraphicsLineItem *>(item);
		if (curLine == NULL) {
			continue;
		}
		scene->removeItem(curLine);
	}

	//! why matrix().reset() doesnt work ?
	QMatrix defaultMatrix ;
	setMatrix(defaultMatrix);

	mIsZoomed = false;
}



void SensorGraph::setNextValue(qreal newValue)
{
	pointsDataProcessor->addNewValue(newValue);
}


void SensorGraph::drawNextFrame()
{
	mainPoint->setPos(pointsDataProcessor->latestPosition());

	//! shifting lines left
	pointsDataProcessor->makeShiftLeft(stepSize);

	foreach (QGraphicsItem *item, scene->items()) {
		QGraphicsLineItem *curLine = qgraphicsitem_cast<QGraphicsLineItem *>(item);
		if (curLine == NULL) {
			continue;
		}
        delete curLine;
	}

	QPen regularPen = QPen(mPenBrush, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	QLineF quantOfGraph;
	for (int i = 0; i < pointsDataProcessor->pointsBase()->size() - 1; i++) {
		quantOfGraph = QLineF(pointsDataProcessor->pointsBase()->at(i)
		, pointsDataProcessor->pointsBase()->at(i + 1));
		scene->addLine(quantOfGraph, regularPen);
	}
}

void SensorGraph::visualTimerEvent()
{
	drawNextFrame();
	if (++mAutoScaleTimer * fpsDelay == autoScaleInterval) {
		mAutoScaleTimer = 0;
		pointsDataProcessor->checkPeaks();
	}
	if (++mUpdateCurrValueTimer * fpsDelay == updateOutputInterval) {
		mUpdateCurrValueTimer = 0;
		mOutputValue = pointsDataProcessor->latestValue();
	}
}

void SensorGraph::drawBackground(QPainter *painter, const QRectF &rect)
{
	QRectF sceneRect = this->sceneRect();

	//! Fill section
	QLinearGradient gradient(sceneRect.bottomLeft(), sceneRect.topRight());
	gradient.setColorAt(0, Qt::black);
	gradient.setColorAt(1, Qt::darkGreen);
	painter->fillRect(scene->sceneRect(), gradient);
	painter->setBrush(Qt::NoBrush);
	painter->setBrush(Qt::CrossPattern);
	painter->drawRect(scene->sceneRect());

	//! Text display section
	QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
					50, 50);
	QString maxDisplay(QString::number(pointsDataProcessor->maxLimit()));
	QString minDisplay(QString::number(pointsDataProcessor->minLimit()));
	QString currentDisplay(QString::number(mOutputValue));

	QFont font = painter->font();
	font.setBold(true);
	font.setPointSize(10);
	painter->setFont(font);
	painter->setPen(Qt::lightGray);
	painter->drawText(textRect.translated(2, 2), maxDisplay);
	painter->drawText(textRect.translated(2, sceneRect.height() - 20), minDisplay);
	painter->drawText(textRect.translated(sceneRect.width() - 30, sceneRect.height() - 20), currentDisplay);
	painter->setPen(Qt::black);
	Q_UNUSED(rect);
}

void SensorGraph::mouseReleaseEvent(QMouseEvent *event)
{
	if (mIsZoomed) {
		zoomOut();
	} else {
		zoomIn();
	}
	mIsZoomed = !mIsZoomed;
}

void SensorGraph::zoomIn()
{
	if (mScaleCoefficient > 5) {
		return;
	}
	QMatrix outputMatrix = matrix();
	outputMatrix.scale(zoomRate, zoomRate);
	setMatrix(outputMatrix);
	mScaleCoefficient++;
}

void SensorGraph::zoomOut()
{
	if (mScaleCoefficient == 0) {
		return;
	}
	QMatrix outputMatrix = matrix();
	outputMatrix.scale(1 / qreal(zoomRate), 1 / qreal(zoomRate));
	setMatrix(outputMatrix);
	mScaleCoefficient--;
}

void SensorGraph::onSensorChange(const int newSensorIndex)
{
	clear();
	switch(newSensorIndex)
	{
	case 3:
	{
		mPenBrush = QBrush(Qt::green);
		break;
	}
	case 2:
	{
		mPenBrush = QBrush(Qt::red);
		break;
	}
	case 1:
	{
		mPenBrush = QBrush(Qt::lightGray);
		break;
	}
	case 0:
	{
		mPenBrush = QBrush(Qt::yellow);
		break;
	}
	default:
	{
		mPenBrush = QBrush(Qt::yellow);
	}
	}
}
