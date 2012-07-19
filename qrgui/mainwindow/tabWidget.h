#pragma once

#include <QtGui/QTabWidget>

namespace qReal {
namespace gui {

class TabWidget : public QTabWidget
{
	Q_OBJECT

public:
	explicit TabWidget(QWidget *parent = 0);

protected:
	void mousePressEvent(QMouseEvent *event);
};

}
}
