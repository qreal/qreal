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

#include <QtCore/QObject>
#include <QtWidgets/QGraphicsView>

#include "editor/editorView.h"
#include "editor/editorViewScene.h"
#include "mainWindow/mainWindow.h"

/** @brief Minimap for the current viewed diagram
*
* Displays the scene of current diagram. The area of the scene, visible in the
* EditorView, displayed on the minimap as a rectangle. Navigation on the scene is possible
* by moving that rectangle with mouse.
*/

class MiniMap : public QGraphicsView
{
	Q_OBJECT
public:
	/// Drag mode is used for dragging a rectangle of the main view
	enum Mode { None, Drag };

	explicit MiniMap(QWidget *parent = 0);

	void init(qReal::MainWindow *window);
	void changeSource(int index);

	void setScene(QGraphicsScene *scene);

public slots:
	/// makes the minimap display only items of the scene
	void showScene();
	void ensureVisible(QList<QRectF> region);

protected:
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);

	void drawForeground(QPainter *painter, const QRectF &rect);
	/// painting out the areas which aren't to be painted on the minimap (not in the scene rect)
	void drawNonExistentAreas(QPainter *painter, const QRectF &rect);
	/// @return list of areas visible on the minimap but not included in the scene rectangle
	QList<QRectF> getNonExistentAreas(const QRectF &rect);

private:
	void setCurrentScene();
	void clear();
	/// @return a rectangle of the scene which is viewed in the editor view
	QRectF getNewRect();

	qReal::MainWindow *mWindow;

	qReal::gui::editor::EditorView *mEditorView;
	/// in the scene coordinates
	QRectF mEditorViewRect;

	Mode mMode;
};
