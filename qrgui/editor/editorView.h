/* Copyright 2007-2016 QReal Research Group
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

#include <QtWidgets/QGraphicsView>

#include "qrgui/editor/editorViewScene.h"
#include "qrgui/editor/private/touchSupportManager.h"
#include "qrgui/editor/private/editorViewMVIface.h"

namespace qReal {
namespace gui {
namespace editor {

class QRGUI_EDITOR_EXPORT EditorView : public QGraphicsView
{
	Q_OBJECT

public:
	EditorView(const models::Models &models
			, Controller &controller
			, const SceneCustomizer &customizer
			, const Id &rootId
			, QWidget *parent = 0);

	const EditorViewMViface &mvIface() const;
	EditorViewMViface &mutableMvIface();
	const EditorViewScene &editorViewScene() const;
	EditorViewScene &mutableScene();

	void setDrawSceneGrid(bool show);
	void ensureElementVisible(const Element * const element);
	void ensureElementVisible(const Element * const element, int xMargin, int yMargin);

signals:
	/// Emitted when for some reason root element was removed and editor must be closed.
	void rootElementRemoved(const QModelIndex &rootGraphicsIndex);

	/// Emitted when current zoom of editor was changed by user.
	void zoomChanged(qreal zoomFactor);

public slots:
	void toggleAntialiasing(bool);
	void zoomIn();
	void zoomOut();
	void zoom(const qreal zoomFactor);

protected:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void scrollContentsBy(int dx, int dy);

	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);

	virtual bool viewportEvent(QEvent *event);
	void focusOutEvent(QFocusEvent* event);
	void focusInEvent(QFocusEvent * event);

private slots:
	void zoomInTime();
	void zoomOutTime();
	void animFinished();

	void setSceneFont();

private:
	void checkGrid();

	void startAnimation(char const *slot);

	EditorViewScene mScene;
	EditorViewMViface mMVIface;
	QPointF mMouseOldPosition;
	bool mWheelPressed;
	view::details::TouchSupportManager mTouchManager;
};

}
}
}
