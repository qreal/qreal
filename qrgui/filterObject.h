#pragma once

#include <QObject>

class FilterObject : public QObject
{
	Q_OBJECT
public:
	explicit FilterObject(QObject *parent = 0);

signals:

public slots:
	bool eventFilter(QObject *object, QEvent *event);
	void triggeredActionActivated();
	void toggledActionActivated(bool status);
	void setStatusCollectUsabilityStatistics(bool status);
	void reportTestStarted();
	void reportTestFinished();
};
