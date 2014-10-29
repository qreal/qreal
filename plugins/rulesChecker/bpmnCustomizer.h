#pragma once

#include <qrgui/plugins/toolPluginInterface/customizer.h>

class BpmnCustomizer : public QObject, public qReal::Customizer
{
	Q_OBJECT
public:
	virtual QString windowTitle() const;
	virtual QString productVersion() const;

	virtual void customizeDocks(qReal::gui::MainWindowDockInterface *dockInterface);

	virtual bool showInterpeterButton() const;
	virtual bool showConnectionRelatedMenus() const;

	virtual QString addExplosionMenuName() const;
	virtual QString changeExplosionMenuName() const;
	virtual QString deleteExplosionMenuName() const;

	virtual QString goToConnectedMenuName() const;

	virtual bool showExpandConnectionAction() const;
	virtual QString expandExplosionActionText() const;
	virtual QString collapseExplosionActionText() const;
};

