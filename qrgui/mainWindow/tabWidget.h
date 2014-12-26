#pragma once

#include <QtWidgets/QTabWidget>

namespace qReal {
namespace gui {

/// Custom class for showing tabs in mainwindow.
class TabWidget : public QTabWidget
{
	Q_OBJECT

public:
	/// Constructor.
	explicit TabWidget(QWidget *parent = 0);

	void setTabUnclosable(int index);

protected:
	void mousePressEvent(QMouseEvent *event);
};

}
}
