#pragma once

#include <QtWidgets/QFrame>

class QToolButton;
class QLineEdit;

namespace qReal {
namespace ui {

/// A text field styled for search. Contains options switcher and clear button.
class SearchLineEdit : public QFrame
{
	Q_OBJECT

public:
	/// A enumeration of possible search options
	enum class SearchOptions
	{
		/// Search is performed ignoring letters case
		CaseInsensitive
		/// Search is performed respectively to letters case
		, CaseSensitive
		/// The text in box is treated as regular expression
		, RegularExpression
	};

	explicit SearchLineEdit(QWidget *parent = nullptr);

signals:
	/// Emitted when the text in the text field is modified.
	/// @param text A ready for matching reglar expression.
	void textChanged(const QRegExp &text);

private:
	QToolButton *initButton(const QIcon &icon, const QString &toolTip);
	void onTextChanged(const QString &text);
	void makeContextMenu();
	void notifyTextChanged();
	QRegExp regexpFromText(const QString &text, SearchOptions option) const;

	QToolButton *mOptionsButton;  // Takes ownership
	QToolButton *mClearButton;  // Takes ownership
	QLineEdit *mLineEdit;  // Takes ownership
	QAction *mCaseInsensitive;  // Takes ownership
	QAction *mCaseSensitive;  // Takes ownership
	QAction *mRegularExpression;  // Takes ownership
	SearchOptions mCurrentOption;
};

}
}
