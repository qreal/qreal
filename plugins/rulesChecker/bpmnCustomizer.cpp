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
