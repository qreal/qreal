#pragma once

#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMouseEventTransition>
#include <QtGui/QMouseEvent>

namespace qReal {

class EditorView;

class MiniMapButton : public QPushButton
{
	Q_OBJECT

public:
	explicit MiniMapButton(EditorView *parent = 0);
	void changeDragState(bool isDragged);
	bool getDragState();

signals:
	void moveMiniMap(QPoint miniMapPos);

private slots:
	void mouseMoveEvent (QMouseEvent *event);

private:
	bool mWidgetMoved;
};

}
