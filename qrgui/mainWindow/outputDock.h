#pragma once

#include <QtWidgets/QDockWidget>

#include <mainWindow/outputWidget.h>

namespace qReal {
namespace gui {

/// Bottom dock that manages displaying of different output widgets.
class OutputDock : public QDockWidget
{
	Q_OBJECT
public:
	explicit OutputDock(QWidget *parent = 0);

	/// Takes control over given widget.
	void addWidget(OutputWidget *widget);

public slots:
	void toggleVisibility();

	void showOutputWidget();

	void hideOutputWidget();

private:
	void showWidget(OutputWidget *widget);

	QList<OutputWidget *> mWidgets;  // Has ownership.
};

}
}
