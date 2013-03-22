#pragma once

#include <QtCore/QTimer>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDockWidget>

#include "../../../qrutils/expressionsParser/expressionsParser.h"

namespace Ui {
	class watchListWindow;
}

int const watchWindowRefreshInterval = 100;

class WatchListWindow : public QDockWidget
{
	Q_OBJECT

public:
	WatchListWindow(utils::ExpressionsParser const *parser, QWidget *parent = 0);
	~WatchListWindow();

private slots:
	void updateVariables();

private:
	Ui::watchListWindow *mUi;
	utils::ExpressionsParser const *mParser;
	QTimer *mTimer;
};
