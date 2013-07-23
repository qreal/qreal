#pragma once

#include <QtCore/QObject>

#include "../../../qrgui/toolPluginInterface/customizer.h"

namespace qRealRobots {

class Customizer : public QObject, public qReal::Customizer
{
	Q_OBJECT

public:
	virtual QString windowTitle() const;
	virtual QIcon applicationIcon() const;
	virtual QString productVersion() const;
	virtual QString aboutText() const;
	virtual void customizeDocks(qReal::gui::MainWindowDockInterface *dockInterface);
	virtual bool showInterpeterButton() const;

private:
	void placePluginWindows(QDockWidget *watchWindow, QWidget *sensorsWidget);
	QDockWidget *produceDockWidget(QString const &title, QWidget *content) const;

	qReal::gui::MainWindowDockInterface *mDockInterface;
};

}
