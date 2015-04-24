/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

const int watchWindowRefreshInterval = 500;

namespace utils {

/// Dock window that shows current values of all variables that are currently known to parser.
class QRUTILS_EXPORT WatchListWindow : public QDockWidget
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param parser - debugger interface to a text language interpreter from which to take variable values.
	/// @param parent - parent of a window as in Qt widget parent/child relations.
	explicit WatchListWindow(const qrtext::DebuggerInterface &interpreter, QWidget *parent = 0);

	/// Constructor left for backwards compatibility with old parser which is still used in visual interpreter.
	/// @param parser - text language parser/interpreter from which to take variable values.
	/// @param parent - parent of a window as in Qt widget parent/child relations.
	explicit WatchListWindow(const utils::ExpressionsParser *parser, QWidget *parent = 0);

	~WatchListWindow() override;

	/// Do not show variables with given names.
	void hideVariables(const QStringList &variableNames);


private slots:
	void updateVariables();

private:
	QString toString(const QVariant &value) const;

	WatchListWindow(const utils::ExpressionsParser * const parser
			, const qrtext::DebuggerInterface * const newParser
			, QWidget *parent);

	Ui::watchListWindow *mUi;
	QTimer mTimer;
	const qrtext::DebuggerInterface * const mNewParser;  // Does not have ownership.
	const utils::ExpressionsParser * const mParser;  // Does not have ownership.
	QSet<QString> mHiddenVariables;
};

}
