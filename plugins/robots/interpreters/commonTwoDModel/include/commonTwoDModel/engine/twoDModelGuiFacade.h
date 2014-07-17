#pragma once

#include <QtWidgets/QWidget>

namespace twoDModel {

namespace view {
class D2ModelWidget;
}

namespace engine {

/// Implemented facade pattern. This class allows to search and get ojects of graphical user interface of
/// robots plugin by their object names, parents and types.
class TwoDModelGuiFacade : public QObject
{
	Q_OBJECT

public:
	explicit TwoDModelGuiFacade(view::D2ModelWidget &d2RobotWidget);

	/// Returns widget, which defined by type and object name.
	Q_INVOKABLE QWidget *widget(QString const &type, QString const &name);

	/// Returns 2d model widget.
	Q_INVOKABLE QWidget *d2ModelWidget();

	/// Returns scene of d2 model widget.
	Q_INVOKABLE QWidget *d2ModelScene();

private:
	view::D2ModelWidget &mD2ModelWidget;
};

}
}
