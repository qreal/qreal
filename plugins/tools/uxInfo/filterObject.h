#pragma once

#include <QtCore/QObject>

namespace uxInfo {

/// Class for user experience information about
/// using menu elements and setting usability
/// testing mode and collection ux information mode.
class FilterObject : public QObject
{
	Q_OBJECT

public:
	explicit FilterObject(QObject *parent = 0);

public slots:
	/// Interception of all events for ux information.
	bool eventFilter(QObject *object, QEvent *event);

	/// Record information about triggered action.
	void triggeredActionActivated();

	/// Record information about toggled action.
	void toggledActionActivated(bool status);

	/// Change collection ux information mode.
	void setStatusCollectUsabilityStatistics(bool status);

	///Record about start of the usability test.
	void reportTestStarted();

	///Record about start of the usability test.
	void reportTestFinished();
};

}
