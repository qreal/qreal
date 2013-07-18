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
	return "0.1.0";
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

QString BpmnCustomizer::forwardConnectionMenuName() const
{
	return QObject::tr("Edit subprocess");
}

QString BpmnCustomizer::backwardConnectionMenuName() const
{
	return QObject::tr("Return to process caller");
}
