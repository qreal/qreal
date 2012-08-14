#pragma once

#include <QtGui/QTabWidget>

namespace qReal {
namespace gui {

/// Custom class for showing tabs in mainwindow.
class TabWidget : public QTabWidget
{
	Q_OBJECT

public:
	/// Constructor.
	explicit TabWidget(QWidget *parent = 0);

protected:
	void mousePressEvent(QMouseEvent *event);
};

}
}
