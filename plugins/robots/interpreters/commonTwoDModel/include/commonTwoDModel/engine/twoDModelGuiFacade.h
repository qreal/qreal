#pragma once

#include <QtWidgets/QWidget>

namespace twoDModel {

namespace view {
class D2ModelWidget;
}

namespace engine {

class TwoDModelGuiFacade : public QObject
{
	Q_OBJECT

public:
	explicit TwoDModelGuiFacade(view::D2ModelWidget &d2RobotWidget);

public slots:
	QWidget *widget(QString const &type, QString const &name);
	QWidget &d2ModelWidget();
	QWidget *d2ModelScene();

private:
	view::D2ModelWidget &mD2ModelWidget;
};

}
}
