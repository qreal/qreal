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

/// Dock window that shows current values of all variables that are currently known to parser.
class QRUTILS_EXPORT WatchListWindow : public QDockWidget
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param parser - debugger interface to a text language interpreter from which to take variable values.
	/// @param parent - parent of a window as in Qt widget parent/child relations.
	explicit WatchListWindow(qrtext::DebuggerInterface const &interpreter, QWidget *parent = 0);

	/// Constructor left for backwards compatibility with old parser which is still used in visual interpreter.
	/// @param parser - text language parser/interpreter from which to take variable values.
	/// @param parent - parent of a window as in Qt widget parent/child relations.
	explicit WatchListWindow(utils::ExpressionsParser const *parser, QWidget *parent = 0);

	~WatchListWindow() override;

	/// Do not show variables with given names.
	void hideVariables(QStringList const &variableNames);


private slots:
	void updateVariables();

private:
	WatchListWindow(utils::ExpressionsParser const * const parser
			, qrtext::DebuggerInterface const * const newParser
			, QWidget *parent);

	Ui::watchListWindow *mUi;
	QTimer mTimer;
	qrtext::DebuggerInterface const * const mNewParser;  // Does not have ownership.
	utils::ExpressionsParser const * const mParser;  // Does not have ownership.

	QSet<QString> mHiddenVariables;
};

}
