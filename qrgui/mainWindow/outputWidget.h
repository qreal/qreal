#pragma once

#include <QtWidgets/QWidget>

namespace qReal {
namespace gui {

/// Base class for widgets displaying something in a bottom dock. Can request its visibility change.
class OutputWidget : public QWidget
{
	Q_OBJECT
public:
	explicit OutputWidget(QWidget *parent = 0);

	/// Initializes real displaying widget.
	void setWidget(QWidget *widget);

signals:
	/// Emitted when a widget wants to change its visibility.
	void toggleVisibility();

	/// Emitted when a widget wants to display something.
	void showRequest();

	/// Emitted when a widget wants to close output dock.
	void hideRequest();
};

}
}