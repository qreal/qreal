#pragma once

#include "../../../qrgui/toolPluginInterface/customizer.h"

namespace qReal {
namespace interpreters {
namespace robots {

class Customizer : public qReal::Customizer
{
public:
	virtual QString windowTitle() const;
	virtual QIcon applicationIcon() const;
	virtual void customizeDocks(gui::MainWindowDockInterface *dockInterface);

	void placePluginWindows(QDockWidget *watchWindow, QWidget *sensorsWidget);

private:
	QDockWidget *produceDockWidget(QString const &title, QWidget *content) const;

	gui::MainWindowDockInterface *mDockInterface;
};

}
}
}
