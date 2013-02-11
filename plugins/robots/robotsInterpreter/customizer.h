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
	virtual void customizeScene(SceneCustomizationInterface *sceneCustomizer);

	void placePluginWindows(QDockWidget *watchWindow, QWidget *sensorsWidget);

public slots:
	void rereadSettings();

private:
	void resetTitlesVisibility();

	QDockWidget *produceDockWidget(QString const &title, QWidget *content) const;

	gui::MainWindowDockInterface *mDockInterface;
	SceneCustomizationInterface *mSceneCustomizer;
};

}
}
}
