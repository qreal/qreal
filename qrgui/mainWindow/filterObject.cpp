#include "filterObject.h"

#include <QtCore/QEvent>
#include <QtWidgets/QAction>

#include "qrutils/uxInfo/uxInfo.h"

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
		utils::UXInfo::reportMenuElements(action->text());
	}
}

void FilterObject::toggledActionActivated(bool status)
{
	QString const statusText = status ? "checked" : "unchecked";
	QAction* const action = static_cast<QAction* const>(sender());
	if (action) {
		QString const statusForUStatistics = "status change on: " + statusText;
		utils::UXInfo::reportMenuElements(action->text(), statusForUStatistics);
	}
}

void FilterObject::setStatusCollectUsabilityStatistics(bool status)
{
	 utils::UXInfo::setStatus(status);
}

void FilterObject::reportTestStarted()
{
	utils::UXInfo::reportTestStarted();
}

void FilterObject::reportTestFinished()
{
	utils::UXInfo::reportTestFinished();
}
