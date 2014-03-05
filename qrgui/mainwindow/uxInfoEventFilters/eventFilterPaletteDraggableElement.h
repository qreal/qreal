#pragma once

#include <QtCore/QObject>

class EventFilterPaletteDraggableElement : public QObject
{
	Q_OBJECT

public:
	explicit EventFilterPaletteDraggableElement(QObject *parent = 0);

public slots:
	/// Interception of all events for ux information.
	bool eventFilter(QObject *object, QEvent *event);

private:
	QString const mouseButton(QEvent *event);
};
