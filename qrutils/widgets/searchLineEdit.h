#pragma once

#include <QtWidgets/QFrame>

class QToolButton;

namespace qReal {
namespace ui {

/// A text field styled for search. Contains options switcher and clear button.
class SearchLineEdit : public QFrame
{
	Q_OBJECT

public:
	explicit SearchLineEdit(QWidget *parent = nullptr);

signals:
	/// Emitted when the text in the text field is modified.
	void textChanged(const QString &text);

private:
	QToolButton *initButton(const QIcon &icon, const QString &toolTip);
	void onTextChanged(const QString &text);

	QToolButton *mActionsButton;  // Takes ownership
	QToolButton *mClearButton;  // Takes ownership
};

}
}
