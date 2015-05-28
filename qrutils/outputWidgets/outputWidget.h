#pragma once

#include <QtWidgets/QWidget>

namespace utils {

/// Base class for widgets displaying something in a bottom dock. Can request its visibility change.
class OutputWidget : public QWidget
{
	Q_OBJECT
public:
	explicit OutputWidget(QWidget *parent = 0);

	/// Initializes real displaying widget.
	virtual void setWidget(QWidget *widget);

	virtual QString title() const = 0;

	virtual QAction *action() = 0;

	virtual QString shortcutName() const = 0;

signals:
	/// Emitted when a widget wants to change its visibility.
	void toggleVisibility();

	/// Emitted when a widget wants to display something.
	void showRequest();

	/// Emitted when a widget wants to close output dock.
	void hideRequest();
};

}
