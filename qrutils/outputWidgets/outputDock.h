#pragma once

#include <QtWidgets/QDockWidget>

#include <qrutils/outputWidgets/outputWidget.h>

namespace utils {

/// Bottom dock that manages displaying of different output widgets.
class OutputDock : public QDockWidget
{
	Q_OBJECT
public:
	explicit OutputDock(QWidget *parent = 0);
	~OutputDock();

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
