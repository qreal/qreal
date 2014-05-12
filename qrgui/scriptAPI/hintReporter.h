#pragma once

#include <QtWidgets/QWidget>
#include "mainwindow/mainWindow.h"
#include "toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace qReal {
namespace gui {

class HintReporter : public QLabel{
	Q_OBJECT

public:
	HintReporter(MainWindow *mainWindow, QString const &message, int const lifeTime);

signals:
	void mousePressEvent();

protected:
	virtual void mousePressEvent(QMouseEvent * event);

private slots:
	void disappear();

private:
	void addHint(QString const &hint);

	QTimer mFadeTimer;
	int mDuration;
};

}
}
