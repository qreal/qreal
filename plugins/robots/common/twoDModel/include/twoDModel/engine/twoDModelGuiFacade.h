#pragma once

#include <QtWidgets/QWidget>

namespace twoDModel {

namespace view {
class TwoDModelWidget;
}

namespace engine {

/// Implemented facade pattern. This class allows to search and manage objects of plugin`s user interface
/// from scripting language by their object names, parents and types.
class TwoDModelGuiFacade : public QObject
{
	Q_OBJECT

public:
	explicit TwoDModelGuiFacade(view::TwoDModelWidget &d2RobotWidget);

	/// Searches and returns widget by type and object name.
	Q_INVOKABLE QWidget *widget(QString const &type, QString const &name);

	/// Returns 2D model window widget.
	Q_INVOKABLE QWidget *twoDModelWidget();

	/// Returns the underlying widget of the 2D model scene.
	Q_INVOKABLE QWidget *twoDModelSceneViewport();

private:
	view::TwoDModelWidget &mD2ModelWidget;
};

}
}
