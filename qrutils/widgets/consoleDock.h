/* Copyright 2015 QReal Research Group
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

#include <QtWidgets/QDockWidget>

class QPlainTextEdit;

namespace qReal {
namespace ui {

/// A widget for displaying console output in main window.
class ConsoleDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit ConsoleDock(const QString &title, QWidget *parent = nullptr);

	/// Returns true if console contains no displayed text.
	bool isEmpty() const;

public slots:
	/// Appends given text to the end of output.
	void print(const QString &text);

	/// Clears output.
	void clear();

private:
	QPlainTextEdit *mOutput;  // Takes ownership
};

}
}
