#pragma once

#include <QtWidgets/QWidget>

#include "details/d2RobotModel/d2ModelWidget.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class RobotsGuiFacade : public QObject
{
	Q_OBJECT

public:
	RobotsGuiFacade(d2Model::D2ModelWidget *d2RobotWidget);

public slots:
	QWidget* widget(QString const &type, QString const &name);
	QWidget* d2ModelWidget();

private:
	d2Model::D2ModelWidget *mD2ModelWidget;
};

}
}
}
}
