#include "filterObject.h"

#include <QtCore/QEvent>
#include <QtWidgets/QAction>

#include "uxInfo.h"

using namespace uxInfo;

FilterObject::FilterObject(QObject *parent)
	: QObject(parent)
{
}

bool FilterObject::eventFilter(QObject *object, QEvent *event)
{
	Q_UNUSED(object);
	Q_UNUSED(event);
	return false;
}

void FilterObject::triggeredActionActivated()
{
	QAction* const action = static_cast<QAction* const>(sender());
	if (action) {
		UXInfo::reportMenuElements(action->text());
	}
}

void FilterObject::toggledActionActivated(bool status)
{
	QString const statusText = status ? "checked" : "unchecked";
	QAction* const action = static_cast<QAction* const>(sender());
	if (action) {
		QString const statusForUStatistics = "status change on: " + statusText;
		UXInfo::reportMenuElements(action->text(), statusForUStatistics);
	}
}

void FilterObject::setStatusCollectUsabilityStatistics(bool status)
{
	UXInfo::setStatus(status);
}

void FilterObject::reportTestStarted()
{
	UXInfo::reportTestStarted();
}

void FilterObject::reportTestFinished()
{
	UXInfo::reportTestFinished();
}
