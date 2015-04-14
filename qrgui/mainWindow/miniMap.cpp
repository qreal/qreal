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

#include "miniMap.h"

MiniMap::MiniMap(QWidget *parent)
		: QGraphicsView(parent)
		, mEditorView(nullptr)
		, mMode(None)
{
}

void MiniMap::init(qReal::MainWindow *window)
{
	mWindow = window;

	setRenderHint(QPainter::Antialiasing, true);

	setInteractive(false);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MiniMap::changeSource(int index)
{
	clear();
	if (index != -1) {
		setCurrentScene();
	} else {
		setScene(nullptr);
	}
}

void MiniMap::setCurrentScene()
{
	mEditorView = mWindow->getCurrentTab();
	if (mEditorView == nullptr) {
		return;
	}

	setScene(mEditorView->scene());
	// can affect zoom - need to change it if we make another desision about it
	connect(mEditorView->scene(), SIGNAL(sceneRectChanged(QRectF)), this, SLOT(showScene()));
}

void MiniMap::setScene(QGraphicsScene *scene)
{
	QGraphicsView::setScene(scene);
	showScene();
}

void MiniMap::showScene()
{
	if (scene() != nullptr) {
		setSceneRect(scene()->sceneRect());
		fitInView(sceneRect(), Qt::KeepAspectRatio);
	}
}

void MiniMap::ensureVisible(QList<QRectF> region)
{
	foreach (QRectF rect, region) {
		fitInView(rect, Qt::KeepAspectRatio);
	}
}

void MiniMap::clear()
{
	mEditorViewRect = QRectF();
	mEditorView = nullptr;
}

QRectF MiniMap::getNewRect()
{
	QRect visibleRect = mEditorView->viewport()->rect();
	QRectF newRect = mEditorView->mapToScene(visibleRect).boundingRect();

	newRect.setTopLeft(newRect.topLeft() - QPointF(2, 2));
	newRect.setBottomRight(newRect.bottomRight() + QPointF(2, 2));

	return newRect;
}

void MiniMap::wheelEvent(QWheelEvent *event)
{
	setInteractive(true);
	QGraphicsView::wheelEvent(event);
	setInteractive(false);
}

void MiniMap::mousePressEvent(QMouseEvent *event)
{
	if (mEditorView != nullptr) {
		mMode = Drag;
		mEditorView->centerOn(mapToScene(event->pos()));
	}
	QGraphicsView::mousePressEvent(event);
}

void MiniMap::mouseMoveEvent(QMouseEvent *event)
{
	if (mEditorView != nullptr && mMode == Drag) {
		mEditorView->centerOn(mapToScene(event->pos()));
	}
	QGraphicsView::mouseMoveEvent(event);
}

void MiniMap::mouseReleaseEvent(QMouseEvent *event)
{
	mMode = None;
	QGraphicsView::mouseReleaseEvent(event);
}

void MiniMap::resizeEvent(QResizeEvent *event)
{
	showScene();
	QGraphicsView::resizeEvent(event);
}

void MiniMap::drawForeground(QPainter *painter, const QRectF &rect)
{
	QGraphicsView::drawForeground(painter, rect);
	painter->setPen(Qt::darkYellow);

	if (mEditorView) {
		mEditorViewRect = getNewRect();
	}
	drawNonExistentAreas(painter, rect);
	painter->drawRect(mEditorViewRect);
}

void MiniMap::drawNonExistentAreas(QPainter *painter, const QRectF &rect)
{
	QList<QRectF> areas = getNonExistentAreas(rect);
	foreach (QRectF area, areas) {
		painter->fillRect(area, Qt::lightGray);
	}
}

QList<QRectF> MiniMap::getNonExistentAreas(const QRectF &rect)
{
	QRectF existent = rect.intersected(sceneRect());

	QList<QRectF> areas;
	areas << QRectF(rect.topLeft(), existent.bottomLeft())
			<< QRectF(rect.topLeft(), QPointF(rect.right(), existent.top()))
			<< QRectF(existent.topRight(), rect.bottomRight())
			<< QRectF(QPointF(rect.left(), existent.bottom()), rect.bottomRight());

	return areas;
}
