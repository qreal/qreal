#pragma once

#include <QDialog>
#include <QTimer>

#include "../../../qrutils/expressionsParser/expressionsParser.h"

namespace Ui {
	class watchListWindow;
}

class watchListWindow : public QDialog
{
	Q_OBJECT

public:
	explicit watchListWindow(const qReal::ExpressionsParser *parser, QWidget *parent = 0);
	~watchListWindow();

public slots:
	void updateVariables();
	
private:
	Ui::watchListWindow *mUi;
	const qReal::ExpressionsParser *mParser;
	QTimer *mTimer;
};
