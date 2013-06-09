#pragma once

#include <QtCore/QObject>

#include "../../../qrgui/toolPluginInterface/customizer.h"

namespace qReal
{
namespace interpreters
{
namespace robots
{

class Customizer : public QObject, public qReal::Customizer
{
	Q_OBJECT

public:
	virtual QString windowTitle() const;
	virtual QIcon applicationIcon() const;
	virtual QString productVersion() const;
	virtual QString aboutText() const;
	virtual void customizeDocks(gui::MainWindowDockInterface *dockInterface);

	void placePluginWindows(QDockWidget *watchWindow, QWidget *sensorsWidget);
	virtual bool showInterpeterButton() const;

private:
	QDockWidget *produceDockWidget(QString const &title, QWidget *content) const;

	gui::MainWindowDockInterface *mDockInterface;
};

}
}
}
