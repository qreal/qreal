#pragma once

#include <QDialog>
#include <QTimer>

#include "expressionsParser/expressionsParser.h"

namespace Ui {
	class watchListWindow;
}

namespace utils {

class QRUTILS_EXPORT watchListWindow : public QDialog
{
	Q_OBJECT

public:
	explicit watchListWindow(const utils::ExpressionsParser *parser, QWidget *parent = 0);
	~watchListWindow();

public slots:
	void updateVariables();

private:
	Ui::watchListWindow *mUi;
	const utils::ExpressionsParser *mParser;
	QTimer *mTimer;
};

}
