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
	return "0.1";
}

bool BpmnCustomizer::showConnectionRelatedMenus() const
{
	return true;
}

QString BpmnCustomizer::addConnectionMenuName() const
{
	return QObject::tr("Set subprocess");
}

QString BpmnCustomizer::deleteConnectionMenuName() const
{
	return QObject::tr("Remove subprocess");
}
