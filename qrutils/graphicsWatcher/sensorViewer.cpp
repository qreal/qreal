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

#include "sensorViewer.h"

#include <qrkernel/exception/exception.h>
#include <qrkernel/logging.h>
#include <qrutils/qRealFileDialog.h>
#include <qrutils/outFile.h>

using namespace utils::sensorsGraph;

SensorViewer::SensorViewer(QWidget *parent)
	: QGraphicsView(parent)
	, mPenBrush(QBrush(Qt::yellow))
	, mFpsInterval(50)
	, mAutoScaleInterval(3000)
	, mUpdateTextInfoInterval(500)
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
	delete mMainPoint;
	delete mMarker;

	delete mPointsDataProcessor;
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
	setCursor(Qt::CrossCursor);

	mScene = new QGraphicsScene(this);
	mScene->setItemIndexMethod(QGraphicsScene::NoIndex);
	mScene->setSceneRect(-200, -160, 205, 160);

	setScene(mScene);
	setRenderHint(QPainter::Antialiasing, false);
	setDragMode(QGraphicsView::NoDrag);

	// This makes information on left side actual
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setCacheMode(CacheNone);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	mMainPoint = new KeyPoint(Qt::yellow);
	mScene->addItem(mMainPoint);

	mMarker = new KeyPoint(Qt::red);
	mScene->addItem(mMarker);
	mMarker->hide();

	mPointsDataProcessor = new PointsQueueProcessor(mScene->sceneRect().height() - 20, mScene->sceneRect().left());
}

void SensorViewer::startJob()
{
	if (!mVisualTimer.isActive()) {
		mVisualTimer.start(mFpsInterval);
	}
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
		if (curLine == nullptr) {
			continue;
		}

		mScene->removeItem(curLine);
	}

	QMatrix defaultMatrix;
	setMatrix(defaultMatrix);
	mScaleCoefficient = 0;
}

void SensorViewer::exportHistory()
{
	QString fileName = QRealFileDialog::getSaveFileName("PlotterCsvSaver"
			, this, tr("Save values history"), QString(), tr("Comma-Separated Values Files (*.csv)"));
	if (fileName.isEmpty()) {
		return;
	}

	if (!fileName.endsWith(".csv")) {
		fileName += ".csv";
	}

	bool fileOpened = false;
	OutFile out(fileName, &fileOpened);
	out() << "time" << ";" << "value" << "\n";
	for (int i = 0; i < mPointsDataProcessor->pointsBase().size(); ++i) {
		const qreal plotValue = mPointsDataProcessor->pointsBase()[i].y();
		out() << i << ";" << mPointsDataProcessor->pointToAbsoluteValue(plotValue) << "\n";
	}

	if (!fileOpened) {
		QLOG_ERROR() << "Couldn`t export sensor values.";
	}
}

void SensorViewer::setNextValue(const qreal newValue)
{
	mPointsDataProcessor->addNewValue(newValue);
}

void SensorViewer::drawNextFrame()
{
	mMainPoint->setPos(mPointsDataProcessor->latestPosition());

	// shifting lines left
	mPointsDataProcessor->makeShiftLeft(stepSize);

	foreach (QGraphicsItem *item, mScene->items()) {
		QGraphicsLineItem *curLine = qgraphicsitem_cast<QGraphicsLineItem *>(item);
		if (curLine == nullptr) {
			continue;
		}

		delete curLine;
	}

	const QPen regularPen = QPen(mPenBrush, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	for (int i = 0; i < mPointsDataProcessor->pointsBase().size() - 1; i++) {
		QLineF quantOfGraph(mPointsDataProcessor->pointsBase()[i]
				, mPointsDataProcessor->pointsBase()[i + 1]);
		mScene->addLine(quantOfGraph, regularPen);
	}
}

void SensorViewer::visualTimerEvent()
{
	drawNextFrame();
	if (++mAutoScaleTimer * mFpsInterval >= mAutoScaleInterval) {
		mAutoScaleTimer = 0;
		mPointsDataProcessor->checkPeaks();
	}

	if (++mUpdateCurrValueTimer * mFpsInterval >= mUpdateTextInfoInterval) {
		mUpdateCurrValueTimer = 0;
		mOutputValue = mPointsDataProcessor->latestValue();
	}
}

void SensorViewer::drawBackground(QPainter *painter, const QRectF &rect)
{
	const int digAfterDot = 1;
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

void SensorViewer::mouseMoveEvent(QMouseEvent *event)
{
	const QPointF pivot = mPointsDataProcessor->pointOfVerticalIntersection(mapToScene(event->pos().x()
			, event->pos().y()));
	qreal valueUnderCursor = mPointsDataProcessor->pointToAbsoluteValue(pivot.y());

	mMarker->setPos(pivot);
	mMarker->show();

	QToolTip::showText(QCursor::pos(), tr("value: ") + QString::number(valueUnderCursor));
}

void SensorViewer::leaveEvent(QEvent *)
{
	mMarker->hide();
}

void SensorViewer::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		zoomIn();
	} else {
		zoomOut();
	}
}

void SensorViewer::zoomIn()
{
	const int maxZoomDegree = 5;
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
	const int noZoom = 0;
	if (mScaleCoefficient == noZoom) {
		return;
	}

	QMatrix outputMatrix = matrix();
	outputMatrix.scale(1 / (zoomRate * 1.0), 1 / (zoomRate * 1.0));
	setMatrix(outputMatrix);
	--mScaleCoefficient;
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

void SensorViewer::configureUserOptions(const int &fpsDelay, const int &autoScaleDelay, const int &textInfoUpdateDelay)
{
	const int maxFpsInterval = 100;
	mFpsInterval = (fpsDelay < maxFpsInterval) ? fpsDelay : maxFpsInterval;
	mAutoScaleInterval = autoScaleDelay;
	mUpdateTextInfoInterval = textInfoUpdateDelay;
	if (mVisualTimer.isActive()) {
		mVisualTimer.stop();
		mVisualTimer.start(mFpsInterval);
	}
}
