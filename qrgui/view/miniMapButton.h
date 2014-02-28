#pragma once

#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QMouseEventTransition>
#include <QMouseEvent>

namespace qReal {

class EditorView;
class EditorViewMViface;
class MainWindow;

class MiniMapButton : public QPushButton
{
	Q_OBJECT

signals:
	void moveMiniMap(QPoint miniMapPos);

public:
	explicit MiniMapButton(EditorView *parent = 0);
	void changeDragState(bool isDragged);
	bool getDragState();

private slots:
	void mouseMoveEvent (QMouseEvent *event);

private:
	bool mWidgetMoved;

};

}
