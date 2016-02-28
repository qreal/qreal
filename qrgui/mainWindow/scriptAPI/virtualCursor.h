/* Copyright 2014-2015 Dmitry Chernov, Dmitry Mordvinov
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

#include <QtWidgets/QLabel>
#include <QtCore/QEvent>

class QPropertyAnimation;

namespace qReal {
namespace gui {

class ScriptAPI;

/// Provides scripting of mouse actions. Implementation moves the widget in shape of mouse cursor
/// on the screen, simulates mouse and wheel events via QApplication::postEvent, has ability to show
/// drag-n-drop operation progress.
/// Mouse navigation target can be specified in global coordinates or with target widget instance.
/// Navigation in hard-coded coordinates is not recommended because widgets can be rendered differently
/// on different platforms.
class VirtualCursor : public QLabel
{
	Q_OBJECT

public:
	explicit VirtualCursor(ScriptAPI &scriptAPI, QWidget *parent = 0);

	/// Moves virtual cursor towards the \a target widget for \a duration milliseconds.
	Q_INVOKABLE void moveTo(const QWidget *target, int duration);

	/// Moves virtual cursor towards the given point in parent coordinates for \a duration milliseconds.
	Q_INVOKABLE void moveToPoint(int x, int y, int duration);

	/// Moves virtual cursor to center of \a target rectangle for \a duration milliseconds.
	Q_INVOKABLE void moveToRect(const QRect &target, int duration);

	/// Moves virtual cursor towards the given cordinates on scene for \a duration milliseconds.
	/// @param target The viewport widget of the scene.
	Q_INVOKABLE void sceneMoveTo(QWidget *target, int duration, int xSceneCoord, int ySceneCoord);

	/// Simulates left mouse button press on widget in the current mouse cursor position.
	/// @param delay How many milliseconds will be delayed after the press event.
	Q_INVOKABLE void leftButtonPress(QWidget *target, int delay = -1);

	/// Simulates left mouse button release on widget in the current mouse cursor position.
	/// @param delay How many milliseconds will be delayed after the release event.
	Q_INVOKABLE void leftButtonRelease(QWidget *target, int delay = -1);

	/// Simulates right mouse button press on widget in the current mouse cursor position.
	/// @param delay How many milliseconds will be delayed after the press event.
	Q_INVOKABLE void rightButtonPress(QWidget *target, int delay = -1);

	/// Simulates right mouse button release on widget in the current mouse cursor position.
	/// @param delay How many milliseconds will be delayed after the release event.
	Q_INVOKABLE void rightButtonRelease(QWidget *target, int delay = -1);

	/// Posts mouse move event to the \a target widget. That is useful when we either want to generate some
	/// hover events or to drag something between mouse press and mouse release events.
	Q_INVOKABLE void moved(QWidget *target);

	/// Draws palette element icon near virtual cursor by name.
	void startDrag(const QIcon &iconName);

	/// Redraws virtual cursor without ant icon.
	void endDrag();

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	void simulateMouse(QObject *reciever, QEvent::Type event, const QPointF &pos, Qt::MouseButton buttons);
	void edgeFinished();

	QPoint widgetPos(QWidget *target) const;
	void animate(const QPoint &from, int toX, int toY, int duration);
	void animate(const QPoint &from, const QPoint &to, int duration);

	ScriptAPI &mScriptAPI;
	QPropertyAnimation *mCursorMoveAnimation;  // Takes ownership

	QPixmap mCursorPixmap;
	QPixmap mDragPixmap;

	bool mRightButtonPressed;
	bool mDragStarted;
};

}
}
