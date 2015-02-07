#pragma once

#include <QtWidgets/QWidget>

namespace qReal {

/// Represents a circle with the border and some icon inside. Border color can be specified
/// with palette foreground color (manually or through brand manager stylesheet)
class CircleWidget : public QWidget
{
public:
	CircleWidget(const QSize &size, const QString &icon, QWidget *parent = 0);

protected:
	virtual void paintEvent(QPaintEvent *) override;
};

}
