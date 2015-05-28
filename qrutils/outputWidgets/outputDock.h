#pragma once

#include <QtWidgets/QTabWidget>

#include <qrutils/outputWidgets/outputWidget.h>

namespace utils {

/// Bottom dock that manages displaying of different output widgets.
class OutputDock : public QTabWidget
{
	Q_OBJECT
public:
	explicit OutputDock(QWidget *parent = 0);
	~OutputDock();

	/// Takes control over given widget.
	void addWidget(OutputWidget *widget);

	void setDock(QWidget *dock);

public slots:
	void toggleVisibility();

	void showOutputWidget();

	void hideOutputWidget();

private slots:
	void closeTab(int index);

private:
	void showWidget(OutputWidget *outputWidget);

	QList<OutputWidget *> mWidgets;  // Has ownership
	QWidget *mDock;  // Doesn't have ownership
};

}
