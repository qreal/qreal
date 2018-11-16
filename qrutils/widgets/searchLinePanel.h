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
class QLineEdit;
class QHBoxLayout;

namespace qReal {
namespace ui {

class SearchLineEdit;

class QRUTILS_EXPORT SearchLinePanel : public QFrame
{
	Q_OBJECT

public:
	/// A enumeration of possible search options
	enum class OperationOptions
	{
		/// Search representation
		Find
		/// Search and replace representation
		, FindAndReplace
		/// Go to line and column representation
		, GoToLineAndColumn
	};

	explicit SearchLinePanel(QWidget *parent);

	void setBackgroundColor(const QColor &color);
	void attachTo(QWidget *parent);
	void detach();

	void setMode(const OperationOptions &option);
	OperationOptions getMode() const;

	QString getTextForReplace() const;
	QString getTextForFind() const;

signals:
	/// Emitted when the text in the find text field is modified.
	/// @param text A ready for matching regular expression.
	void findTextChanged(const QRegExp &text);
	/// Emitted when the text in the replace text field is modified.
	void replaceTextChanged(const QString &text);
	void nextPressed();
	void replacePressed();
	void closePressed();

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;

private:
	void moveAndAlign();

	SearchLineEdit *mSearchLineEdit;
	QPushButton *mNextButton;
	QPushButton *mCloseButton;
	QPushButton *mReplaceButton;
	QLineEdit *mReplaceLineEdit;

	OperationOptions mCurrentOption;
};

}
}
