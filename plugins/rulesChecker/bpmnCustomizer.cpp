#include "bpmnCustomizer.h"

void BpmnCustomizer::customizeDocks(qReal::gui::MainWindowDockInterface *dockInterface)
{
	dockInterface->logicalModelDock()->hide();
}

bool BpmnCustomizer::showInterpeterButton() const
{
	return false;
}

QString BpmnCustomizer::windowTitle() const
{
	return QObject::tr("QReal:BP") + " " + productVersion();
}

QString BpmnCustomizer::productVersion() const
{
	return "0.3.1";
}

bool BpmnCustomizer::showConnectionRelatedMenus() const
{
	return true;
}

QString BpmnCustomizer::addExplosionMenuName() const
{
	return QObject::tr("Set subprocess");
}

QString BpmnCustomizer::changeExplosionMenuName() const
{
	return QObject::tr("Change subprocess");
}

QString BpmnCustomizer::deleteExplosionMenuName() const
{
	return QObject::tr("Remove subprocess");
}

QString BpmnCustomizer::goToConnectedMenuName() const
{
	return QObject::tr("Edit subprocess");
}

bool BpmnCustomizer::showExpandConnectionAction() const
{
	return true;
}

QString BpmnCustomizer::expandExplosionActionText() const
{
	return QObject::tr("Expand subprocess");
}

QString BpmnCustomizer::collapseExplosionActionText() const
{
	return QObject::tr("Collapse subprocess");
}
