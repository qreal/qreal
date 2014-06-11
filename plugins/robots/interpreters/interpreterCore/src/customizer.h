#pragma once

#include <QtCore/QObject>

#include <qrgui/toolPluginInterface/customizer.h>

namespace interpreterCore {

/// Configures QReal's main GUI module disabling some unneeded stuff and customizing such
/// aspects as window title, window icon and so on.
class Customizer : public qReal::Customizer
{
public:
	QString windowTitle() const override;
	QIcon applicationIcon() const override;
	QImage applicationLogo() const override;
	QString productVersion() const override;
	QString aboutText() const override;
	QString examplesDirectory() const override;

	void customizeDocks(qReal::gui::MainWindowDockInterface *dockInterface) override;

	/// Embeds the given widget into main window`s left dock panel
	void placeDevicesConfig(QWidget *devicesWidget);

	/// Embeds the given widgets into main window`s left dock panel tabifying them together.
	void placeWatchPlugins(QDockWidget *watchWindow, QWidget *graphicsWatch);

	bool showInterpeterButton() const override;

	QString userPaletteTitle() const override;
	QString userPaletteDescription() const override;

private:
	void placePluginWindows(QDockWidget *watchWindow, QWidget *sensorsWidget);
	QDockWidget *produceDockWidget(QString const &title, QWidget *content) const;

	qReal::gui::MainWindowDockInterface *mDockInterface;
};

}
