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

void FilterObject::actionHovered()
{
	QAction* const action = static_cast<QAction* const>(sender());
	if (action) {
		QString const actionText = action->text();
		QString const userAction = QString::fromUtf8("Навести на пункт меню — название: ")
				+ actionText
				+ QString::fromUtf8("|");
		utils::UXInfo::instance()->reportUserAction(userAction);
	}
}

void FilterObject::actionTriggered()
{
	QAction* const action = static_cast<QAction* const>(sender());
	if (action) {
		QString const actionText = action->text();

		QString const userAction = QString::fromUtf8("Нажать на пункт меню — название: ")
				+ actionText
				+ QString::fromUtf8("|");
		utils::UXInfo::instance()->reportUserAction(userAction);
	}
}

void FilterObject::actionToggled(bool toStatus)
{
	QAction* const action = static_cast<QAction* const>(sender());
	if (action) {
		QString const actionText = action->text();

		QString const userAction = QString::fromUtf8("Нажать на пункт меню — название: ")
				+ actionText
				+ QString::fromUtf8("|состояние: ")
				+ ((toStatus) ? QString::fromUtf8("включить|") : QString::fromUtf8("выключить|"));
		utils::UXInfo::instance()->reportUserAction(userAction);
	}
}

void FilterObject::edgeContextMenuActionHovered()
{
	QAction* const action = static_cast<QAction* const>(sender());
	if (action) {
		QString const actionText = action->text();
		QString const userAction = QString::fromUtf8("Навести на пункт контекстного меню линка — название: ")
				+ actionText
				+ QString::fromUtf8("|");
		utils::UXInfo::instance()->reportUserAction(userAction);
	}
}

void FilterObject::edgeContextMenuActionTriggered()
{
	QAction* const action = static_cast<QAction* const>(sender());
	if (action) {
		QString const actionText = action->text();

		QString const userAction = QString::fromUtf8("Нажать на пункт контекстного меню линка — название: ")
				+ actionText
				+ QString::fromUtf8("|");
		utils::UXInfo::instance()->reportUserAction(userAction);
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
