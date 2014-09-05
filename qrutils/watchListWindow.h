#pragma once

#include <QtCore/QTimer>
#include <QtCore/QSet>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDockWidget>

#include <qrtext/debuggerInterface.h>

#include "expressionsParser/expressionsParser.h"

namespace Ui {
	class watchListWindow;
}

int const watchWindowRefreshInterval = 500;

namespace utils {

class QRUTILS_EXPORT WatchListWindow : public QDockWidget
{
	Q_OBJECT

public:
	WatchListWindow(qrtext::DebuggerInterface &newParser, QWidget *parent = 0);

	void hideVariables(QStringList const &variableNames);

	~WatchListWindow();

private slots:
	void updateVariables();

private:
	Ui::watchListWindow *mUi;
	QTimer *mTimer;
	qrtext::DebuggerInterface &mNewParser;

	QSet<QString> mHiddenVariables;
};

}
