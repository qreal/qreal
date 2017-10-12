/* Copyright 2017 CyberTech Labs Ltd.
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

#include <qrutils/utilsDeclSpec.h>

#include <QtWidgets/QFrame>

class QPushButton;

namespace qReal {
namespace ui {

class SearchLineEdit;

class QRUTILS_EXPORT SearchLinePanel : public QFrame
{
	Q_OBJECT

public:
	explicit SearchLinePanel(QWidget *parent);

	void setBackgroundColor(const QColor &color);
	void attachTo(QWidget *parent);
	void detach();

signals:
	/// Emitted when the text in the text field is modified.
	/// @param text A ready for matching regular expression.
	void textChanged(const QRegExp &text);
	void nextPressed();
	void closePressed();

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;

private:
	void moveAndAlign();

	SearchLineEdit *mSearchLineEdit;
	QPushButton *mNextButton;
	QPushButton *mCloseButton;
};

}
}
