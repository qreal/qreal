#pragma once

#include <QtWidgets/QWidget>

namespace qReal {

/// Represents a circle with the border a some widget inside
class CircleWidget : public QWidget
{
public:
	CircleWidget(QSize const &size, QString const &icon, QWidget *parent = 0);

protected:
	virtual void paintEvent(QPaintEvent *) override;
};

}
