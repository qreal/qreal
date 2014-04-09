#pragma once

#include <QtCore/QObject>

#include <qrgui/toolPluginInterface/customizer.h>

namespace interpreterCore {

class Customizer : public qReal::Customizer
{
public:
	virtual QString windowTitle() const;
	virtual QIcon applicationIcon() const;
	virtual QString productVersion() const;
	virtual QString aboutText() const;
	virtual QString examplesDirectory() const;

	virtual void customizeDocks(qReal::gui::MainWindowDockInterface *dockInterface);
	void placeDevicesConfig(QWidget *devicesWidget);
	void placeWatchPlugins(QDockWidget *watchWindow, QWidget *graphicsWatch);

	virtual bool showInterpeterButton() const;

	virtual QString userPaletteTitle() const;
	virtual QString userPaletteDescription() const;

private:
	void placePluginWindows(QDockWidget *watchWindow, QWidget *sensorsWidget);
	QDockWidget *produceDockWidget(QString const &title, QWidget *content) const;

	qReal::gui::MainWindowDockInterface *mDockInterface;
};

}
