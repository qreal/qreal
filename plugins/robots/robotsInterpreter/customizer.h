#pragma once

#include <QtCore/QObject>

#include <qrgui/toolPluginInterface/customizer.h>

namespace qReal {
namespace interpreters {
namespace robots {

class Customizer : public QObject, public qReal::Customizer
{
	Q_OBJECT

public:
	virtual QString windowTitle() const;
	virtual QIcon applicationIcon() const;
	virtual QString productVersion() const;
	virtual QString aboutText() const;
	virtual QString examplesDirectory() const;

	virtual void customizeDocks(gui::MainWindowDockInterface *dockInterface);
	void placeSensorsConfig(QWidget *sensorsWidget);
	void placeWatchPlugins(QDockWidget *watchWindow, QWidget *graphicsWatch);

	virtual bool showInterpeterButton() const;

	virtual QString userPaletteTitle() const;
	virtual QString userPaletteDescription() const;

private:
	QDockWidget *produceDockWidget(QString const &title, QWidget *content) const;

	gui::MainWindowDockInterface *mDockInterface;
};

}
}
}
