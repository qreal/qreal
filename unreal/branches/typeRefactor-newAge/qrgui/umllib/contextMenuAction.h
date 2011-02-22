#pragma once

#include <QtGui/QAction>

namespace UML {

// saves cursor's position within the scene
// could be useful to call Element's event handlers from the scene

class ContextMenuAction: public QAction {
	Q_OBJECT
public:
	ContextMenuAction(QString const &text, QObject *parent);
	void setEventPos(QPointF const &pos);
	QPointF eventPos() const;
private slots:
	void trigger();
signals:
	void triggered(QPointF const &pos);
private:
	QPointF mEventPos;
};

}
