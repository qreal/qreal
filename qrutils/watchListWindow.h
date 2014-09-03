#pragma once

#include <QtCore/QTimer>
#include <QtCore/QSet>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDockWidget>

#include <qrtext/lua/luaToolbox.h>

#include "expressionsParser/expressionsParser.h"

namespace Ui {
	class watchListWindow;
}

int const watchWindowRefreshInterval = 100;

namespace utils {

class QRUTILS_EXPORT WatchListWindow : public QDockWidget
{
	Q_OBJECT

public:
	WatchListWindow(utils::ExpressionsParser const *parser, qrtext::lua::LuaToolbox &newParser, QWidget *parent = 0);

	void hideVariables(QStringList const &variableNames);

	~WatchListWindow();

private slots:
	void updateVariables();

private:
	Ui::watchListWindow *mUi;
	utils::ExpressionsParser const *mParser;
	QTimer *mTimer;
	qrtext::lua::LuaToolbox &mNewParser;

	QSet<QString> mHiddenVariables;
};

}
