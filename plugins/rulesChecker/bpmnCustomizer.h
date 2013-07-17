#pragma once

#include "../../qrgui/toolPluginInterface/customizer.h"

class BpmnCustomizer : public QObject, public qReal::Customizer
{
	Q_OBJECT
public:
	virtual QString windowTitle() const;
	virtual QString productVersion() const;

	virtual void customizeDocks(qReal::gui::MainWindowDockInterface *dockInterface);

	virtual bool showInterpeterButton() const;
	virtual bool showConnectionRelatedMenus() const;
};

