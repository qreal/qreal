#pragma once

#include <QtWidgets/QAction>

// saves cursor's position within the scene
// could be useful to call Element's event handlers from the scene

class ContextMenuAction: public QAction
{
	Q_OBJECT

public:
	ContextMenuAction(const QString &text, QObject *parent);
	void setEventPos(const QPointF &pos);
	QPointF eventPos() const;

private slots:
	void trigger();

signals:
	void triggered(const QPointF &pos);

private:
	QPointF mEventPos;
};
