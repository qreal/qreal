#include "sensorViewer.h"

using namespace qReal::interpreters::robots::sensorsGraph;

SensorViewer::SensorViewer(QWidget *parent)
	: QGraphicsView(parent)
	, mPenBrush(QBrush(Qt::yellow))
	, mScaleCoefficient(0)
	, mAutoScaleTimer(0)
	, mUpdateCurrValueTimer(0)
	, mOutputValue(0)
{
	initGraphicsOutput();

	connect(&mVisualTimer, SIGNAL(timeout()), this, SLOT(visualTimerEvent()));
}

SensorViewer::~SensorViewer()
{
	mScene->removeItem(mainPoint);
	delete mPointsDataProcessor;
	delete mainPoint;
	delete mScene;
}

void SensorViewer::resizeEvent(QResizeEvent *event)
{
	Q_UNUSED(event);
	mScene->setSceneRect(sceneRect());
	mPointsDataProcessor->setViewParams(mScene->sceneRect().height() - 20, mScene->sceneRect().left());
}

void SensorViewer::initGraphicsOutput()
{
	mScene = new QGraphicsScene(this);
	mScene->setItemIndexMethod(QGraphicsScene::NoIndex);
	mScene->setSceneRect(-200, -160, 205, 160);

	setScene(mScene);
	setRenderHint(QPainter::Antialiasing, false);
	setDragMode(QGraphicsView::ScrollHandDrag);

	// This makes information on left side actual
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setCacheMode(CacheNone);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	mainPoint = new KeyPoint;
	mScene->addItem(mainPoint);

	mPointsDataProcessor = new PointsQueueProcessor(mScene->sceneRect().height() - 20, mScene->sceneRect().left());
}

void SensorViewer::startJob()
{
	mVisualTimer.start(fpsDelay);
}

void SensorViewer::stopJob()
{
	mVisualTimer.stop();
}

void SensorViewer::clear()
{
	mPointsDataProcessor->clearData();

	foreach (QGraphicsItem *item, mScene->items()) {
		QGraphicsLineItem *curLine = qgraphicsitem_cast<QGraphicsLineItem *>(item);
		if (curLine == NULL) {
			continue;
		}
		mScene->removeItem(curLine);
	}

	// why matrix().reset() doesnt work ?
	QMatrix defaultMatrix;
	setMatrix(defaultMatrix);
}

void SensorViewer::setNextValue(qreal const newValue)
{
	mPointsDataProcessor->addNewValue(newValue);
}

void SensorViewer::drawNextFrame()
{
	mainPoint->setPos(mPointsDataProcessor->latestPosition());

	// shifting lines left
	mPointsDataProcessor->makeShiftLeft(stepSize);

	foreach (QGraphicsItem *item, mScene->items()) {
		QGraphicsLineItem *curLine = qgraphicsitem_cast<QGraphicsLineItem *>(item);
		if (curLine == NULL) {
			continue;
		}
		delete curLine;
	}

	QPen const regularPen = QPen(mPenBrush, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	QLineF quantOfGraph;
	for (int i = 0; i < mPointsDataProcessor->pointsBase()->size() - 1; i++) {
		quantOfGraph = QLineF(mPointsDataProcessor->pointsBase()->at(i)
							  , mPointsDataProcessor->pointsBase()->at(i + 1));
		mScene->addLine(quantOfGraph, regularPen);
	}
}

void SensorViewer::visualTimerEvent()
{
	drawNextFrame();
	if (++mAutoScaleTimer * fpsDelay == autoScaleInterval) {
		mAutoScaleTimer = 0;
		mPointsDataProcessor->checkPeaks();
	}
	if (++mUpdateCurrValueTimer * fpsDelay == updateOutputInterval) {
		mUpdateCurrValueTimer = 0;
		mOutputValue = mPointsDataProcessor->latestValue();
	}
}

void SensorViewer::drawBackground(QPainter *painter, const QRectF &rect)
{
	int const digAfterDot = 1;
	QRectF sceneRect = this->sceneRect();

	// Fill section
	QLinearGradient gradient(sceneRect.bottomLeft(), sceneRect.topRight());
	gradient.setColorAt(0, Qt::black);
	gradient.setColorAt(1, Qt::darkGreen);
	painter->fillRect(mScene->sceneRect(), gradient);
	painter->setBrush(Qt::NoBrush);
	painter->setBrush(Qt::CrossPattern);
	painter->drawRect(mScene->sceneRect());

	// Text display section
	QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4, 50, 50);
	QString maxDisplay(QString::number(mPointsDataProcessor->maxLimit(), 'f', digAfterDot));
	QString minDisplay(QString::number(mPointsDataProcessor->minLimit(), 'f', digAfterDot));
	QString currentDisplay(QString::number(mOutputValue, 'f', digAfterDot));

	QFont font = painter->font();
	font.setBold(true);
	font.setPointSize(10);
	painter->setFont(font);
	painter->setPen(Qt::lightGray);
	painter->drawText(textRect.translated(2, 2), maxDisplay);
	painter->drawText(textRect.translated(2, sceneRect.height() - 20), minDisplay);
	painter->drawText(textRect.translated(sceneRect.width() - 35, sceneRect.height() - 20), currentDisplay);
	painter->setPen(Qt::black);
	Q_UNUSED(rect);
}

void SensorViewer::zoomIn()
{
	int const maxZoomDegree = 5;
	if (mScaleCoefficient > maxZoomDegree) {
		return;
	}
	QMatrix outputMatrix = matrix();
	outputMatrix.scale(zoomRate, zoomRate);
	setMatrix(outputMatrix);
	mScaleCoefficient++;
}

void SensorViewer::zoomOut()
{
	int const noZoom = 0;
	if (mScaleCoefficient == noZoom) {
		return;
	}
	QMatrix outputMatrix = matrix();
	outputMatrix.scale(1 / qreal(zoomRate), 1 / qreal(zoomRate));
	setMatrix(outputMatrix);
	mScaleCoefficient--;
}

void SensorViewer::onSensorChange()
{
	clear();

	if (mPenBrush.color().toCmyk() == QColor(Qt::yellow).toCmyk()) {
		mPenBrush = QBrush(Qt::green);
	} else {
		mPenBrush = QBrush(Qt::yellow);
	}
}
