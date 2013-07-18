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

	virtual QString addConnectionMenuName() const;
	virtual QString deleteConnectionMenuName() const;

	virtual QString forwardConnectionMenuName() const;
	virtual QString backwardConnectionMenuName() const;

	virtual bool showExpandConnectionAction() const;
	virtual QString expandConnectionActionText() const;
	virtual QString collapseConnectionActionText() const;
};

